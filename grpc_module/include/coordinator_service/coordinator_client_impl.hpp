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

			logger_->info("{0} connected to {1}", class_name()
				            , context_.address().get());
		}

		void stop() override
		{			
			for (auto it : handlers_)
				it.second.completion_queue->Shutdown();
			handlers_.clear();

			logger_->info("{0} stopped", class_name());
		}	

		bool connect_request(const DataTypes::ConnectMsg& request) override
		{			
			auto queue = utils::get_completion_queue<AsyncConnectCall>(handlers_);
			if (queue == nullptr)
				return;

			auto call = new AsyncConnectCall;
			utils::set_deadline(call->context, REQUEST_DEADLINE);
			call->reader = stub_->AsyncConnect(&call->context, request, queue);
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));
						
			try
			{
				auto result = utils::get_result(call->promise);			
				return result;
			}
			catch (std::exception& exception)	{
				logger_->error("Connect request exception : {0}", exception.what());		
				return false;
			}			
		}
			
		//TODO not here
		std::pair<std::string, std::string> get_service_id_metadata() const
		{
			auto guid = context_.unit_context()->configuration().service_uuid();
			return std::pair<std::string, std::string>("id", guid);
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
			}		
		}

		void update_devices(const DataTypes::DeviceUpdate& request) const override
		{
			auto queue = utils::get_completion_queue<AsyncUpdateDevicesCall>(handlers_);
			if (queue == nullptr)
				return;

			auto call = new AsyncUpdateDevicesCall;
			utils::set_metadata(call->context, get_service_id_metadata());
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

			return utils::get_result(call->promise);
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
			utils::set_metadata(call->context, get_service_id_metadata());
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

		std::unique_ptr<Services::CoordinatorService::Stub> stub_;
		std::shared_ptr<grpc::Channel>                   channel_;

		ClientContext context_;
		ClientRequestHandlers handlers_;

		grpc::DynamicThreadPool thread_pool_;


		std::shared_ptr<contracts::common::Logger> logger_;

		const int REQUEST_DEADLINE = 1;

		CoordinatorClientImpl(const CoordinatorClientImpl&) = delete;
		CoordinatorClientImpl& operator=(const CoordinatorClientImpl&) = delete;
	};
}

#endif