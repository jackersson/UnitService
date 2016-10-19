#ifndef CoordinatorClientImpl_INCLUDED
#define CoordinatorClientImpl_INCLUDED

#include <grpc++/grpc++.h>
#include <contracts/services/iservice.hpp>
#include "client_context.hpp"
#include <services/coordinator_service.grpc.pb.h>
#include <database_service/database_client_calls.hpp>
#include "coordinator_client_calls.hpp"

#include <service_utils.hpp>

#include <src/cpp/server/dynamic_thread_pool.h>
#include <contracts/services/icoordinator_api.hpp>
#include <features/heartbeat.hpp>

using grpc::ServerBuilder;

namespace grpc_services
{
	class CoordinatorClientImpl : public contracts::services::IService
		                          , public contracts::services::IDatabaseApi
	                           	, public contracts::services::ICoordinatorApi
	{
	public:
		explicit CoordinatorClientImpl(const ClientContext& context)
			: active_(false), context_(context)
			, thread_pool_(0)
			, need_connect_(true)
		{
			logger_ = context.unit_context()->logger();
			CoordinatorClientImpl::init();
		}

		~CoordinatorClientImpl(){
			CoordinatorClientImpl::stop();
			logger_->info("{0} destroyed", class_name());
		}

		void connect()
		{
			auto address = context_.address().get();
			logger_->info("Try create channel {0}", address);

			
			channel_ = CreateChannel(address, grpc::InsecureChannelCredentials());
			stub_ = Services::CoordinatorService::NewStub(channel_);			
		}

		void start_heartbeat()
		{
			if (heartbeat_impl_ != nullptr)
				return;
			logger_->info("{0} Try start hearbeat", class_name());
			const auto& config = context_.unit_context()->configuration();
			auto service_uuid = config.service_uuid();
			features::HeartbeatContext context;
			context.logger = logger_;
		
			features::Heartbeat::fill_message(service_uuid
				, DataTypes::ServiceType::Unit_Service
				, heart_beat_message_);

			context.action = std::bind(&CoordinatorClientImpl::heart_beat
				, this, heart_beat_message_);

			context.can_perform = std::bind(&CoordinatorClientImpl::can_hearbeat
				                              , this);

			heartbeat_impl_ = std::make_unique<features::Heartbeat>(context);

			heartbeat_impl_->start();
		}

	

		bool can_hearbeat()
		{
			return true;
		}

		void stop_heartbeat()
		{
			if (heartbeat_impl_ != nullptr)
			  heartbeat_impl_->stop();
			logger_->info("{0} hearbeat stopped", class_name());
		}

		void init() override
		{
			connect();
				
			add_call_handler<AsyncConnectCall>();
			add_call_handler<AsyncHeartbeatCall>();
			add_call_handler<AsyncUpdateDevicesCall>();
			add_call_handler<AsyncGetRequestCall>();
			add_call_handler<AsyncCommitRequestCall>();
		}

		void start() override
		{
			if (active_)
				return;

			active_ = true;
			for (auto handler : handlers_)
				thread_pool_.Add(handler.second.callback);

			//start_heartbeat();
			logger_->info("{0} connected to {1}", class_name()
				            , context_.address().get());
		}

		void stop() override
		{
			stop_heartbeat();
			for (auto it : handlers_)
				it.second.completion_queue->Shutdown();
			handlers_.clear();

			logger_->info("{0} stopped", class_name());
		}	

		void connect_request(const DataTypes::ConnectMsg& request) override
		{			
			connect_message_.CopyFrom(request);
			auto queue = utils::get_completion_queue<AsyncConnectCall>(handlers_);
			if (queue == nullptr)
				return;

			auto call = new AsyncConnectCall;
			set_deadline(call->context, 1); // TODO to consts
			call->reader = stub_->AsyncConnect(&call->context, request, queue);
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));
						
			try
			{
				auto result = utils::get_result(call->promise);
				logger_->info("Connected to coordinator successfully. Result {0}", result);
				start_heartbeat(); //TODO not here

				need_connect_ = false;
				if (connect_impl_ != nullptr)
					connect_impl_->stop();
			}
			catch (std::exception& exception)	{
				logger_->error("Coordinator client can't connect. {0}", exception.what());
				try_connect();
			}			
		}

		//TODO not here
		void set_deadline( grpc::ClientContext& context, uint32_t seconds)
		{
			auto deadline =
				std::chrono::system_clock::now() + std::chrono::seconds(seconds);
			context.set_deadline(deadline);
		}

		//TODO not here
		void set_metadata(grpc::ClientContext& context
		   , const std::vector<std::pair<std::string, std::string>>& metadata ) const
		{
	  	for (auto item : metadata)
			  context.AddMetadata(item.first, item.second);
		}

		void set_metadata(grpc::ClientContext& context
		              	, const std::pair<std::string, std::string>& metadata) const
		{
			context.AddMetadata(metadata.first, metadata.second);
		}

		//TODO not here
		std::pair<std::string, std::string> get_service_id_metadata() const
		{
			auto guid = context_.unit_context()->configuration().service_uuid();
			return std::pair<std::string, std::string>("id", guid);
		}

		void try_connect()
		{
			if (connect_impl_ != nullptr)
				return;

			features::HeartbeatContext context;
			context.logger = logger_;

			context.action = std::bind(&CoordinatorClientImpl::connect_request
				                      , this, connect_message_);

			context.can_perform = std::bind(&CoordinatorClientImpl::need_connect, this);

			connect_impl_ = std::make_unique<features::Heartbeat>(context);
			connect_impl_->start();
		}

		bool need_connect()
		{
			return need_connect_;
		}

		void heart_beat(const DataTypes::HeartbeatMessage& request) const override
		{
			auto queue = utils::get_completion_queue<AsyncHeartbeatCall>(handlers_);
			if (queue == nullptr)
				return;

			auto call = new AsyncHeartbeatCall;
			call->reader = stub_->AsyncHeartbeat(&call->context, request, queue);
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));

			try	{
				utils::get_result(call->promise);				
			}
			catch (std::exception& exception) {
				//TODO handle broken promise as no response from server
				logger_->error("Coordinator client {0}", exception.what());
				//connection_failed();
			}		
		}

		void update_devices(const DataTypes::DeviceUpdate& request) const override
		{
			auto queue = utils::get_completion_queue<AsyncUpdateDevicesCall>(handlers_);
			if (queue == nullptr)
				return;

			auto call = new AsyncUpdateDevicesCall;
			set_metadata(call->context, get_service_id_metadata());
			call->reader = stub_->AsyncUpdateDevices(&call->context, request, queue);
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));
		}

		std::shared_ptr<DataTypes::GetResponse>
			get(const DataTypes::GetRequest& request) override
		{
			DataTypes::MessageBytes message;
			utils::to_bytes(request, message);

			auto queue = utils::get_completion_queue<AsyncGetRequestCall>(handlers_);
			if (queue == nullptr)
				return nullptr;

			auto call = new AsyncGetRequestCall;
			call->reader = stub_->AsyncGet(&call->context, message, queue);
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));

			return utils::get_result(call->promise_);
		}

		std::shared_ptr<DataTypes::CommitResponse>
			commit(const DataTypes::CommitRequest& request) override
		{
			DataTypes::MessageBytes message;
			utils::to_bytes(request, message);

			auto it = handlers_.find(typeid(AsyncCommitRequestCall).name());
			if (it == handlers_.end())
				return nullptr;

			auto call = new AsyncCommitRequestCall;
			set_metadata(call->context, get_service_id_metadata());
			call->reader = stub_->AsyncGet(&call->context, message, it->second.completion_queue.get());
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));

			//TODO add try catch with timeout
			return utils::get_result(call->promise);
		}
		
	private:
		template<typename T>
		void add_call_handler()
		{
			auto cq = std::make_shared<grpc::CompletionQueue>();

			auto callback = std::bind(&CoordinatorClientImpl::async_complete_rpc<T>
				, this, cq.get());
			handlers_.insert(
				std::pair<std::string, ClientRequestHandler>(
					typeid(T).name(), ClientRequestHandler(cq, callback)));
		}

		template <typename T>
		void async_complete_rpc(grpc::CompletionQueue* queue) const
		{
			void* got_tag;
			auto  ok = false;

			while (queue->Next(&got_tag, &ok))
			{
				auto call = static_cast<T*>(got_tag);
				try
				{
					if (call->status.ok())					
						call->process();					
					//else
						//logger_->error("{0} rpc failed", class_name());
				}
				catch (std::exception& ex) {
					logger_->error(ex.what());
				}
				delete call;
			}
		}

		std::string class_name() const {
			return typeid(CoordinatorClientImpl).name();
		}

		bool active_;
		bool need_connect_;

		//TODO increase value
		const uint16_t MAX_CONNECTION_FAILS_COUNT = 3;

		std::unique_ptr<Services::CoordinatorService::Stub> stub_;
		std::shared_ptr<grpc::Channel>                   channel_;

		ClientContext context_;
		ClientRequestHandlers handlers_;

		grpc::DynamicThreadPool thread_pool_;


		std::shared_ptr<contracts::common::Logger> logger_;

		std::unique_ptr<features::Heartbeat> heartbeat_impl_;
		DataTypes::HeartbeatMessage heart_beat_message_;

		std::unique_ptr<features::Heartbeat> connect_impl_;
		DataTypes::ConnectMsg connect_message_;

		CoordinatorClientImpl(const CoordinatorClientImpl&) = delete;
		CoordinatorClientImpl& operator=(const CoordinatorClientImpl&) = delete;
	};
}

#endif