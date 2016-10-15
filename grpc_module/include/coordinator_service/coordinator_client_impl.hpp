#ifndef CoordinatorClientImpl_INCLUDED
#define CoordinatorClientImpl_INCLUDED

#include <grpc++/grpc++.h>
#include <contracts/services/iservice.hpp>
#include <services/database_service.grpc.pb.h>
#include "client_context.hpp"
#include <services/coordinator_service.grpc.pb.h>
#include <database_service/database_client_calls.hpp>
#include "coordinator_client_calls.hpp"

#include <service_utils.hpp>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

using grpc::ServerBuilder;

namespace grpc_services
{
	class CoordinatorClientImpl : public contracts::services::IService
		                          , public contracts::services::IDatabaseApi
	{
	public:
		explicit CoordinatorClientImpl(const ClientContext& context)
			: active_(false), context_(context)
		{
			threads_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
			logger_ = context.unit_context()->logger();
			CoordinatorClientImpl::init();
		}

		~CoordinatorClientImpl(){
			CoordinatorClientImpl::stop();
			logger_->info("{0} destroyed", class_name());
		}

		void init() override
		{
			auto address = context_.address().get();
			logger_->info("Try create channel {0}", address);

			channel_ = CreateChannel(address, grpc::InsecureChannelCredentials());
			stub_ = Services::CoordinatorService::NewStub(channel_);
			
			//add_call_handler<AsyncGetRequestCall>();		
		}

		void start() override
		{
			if (active_)
				return;

			active_ = true;

			for (auto handler : handlers_)
				io_service_.post(handler.second.callback);

			logger_->info("{0} connected to {1}", class_name()
				            , context_.address().get());
		}

		void stop() override
		{
			io_service_.stop();
			for (auto it : handlers_)
				it.second.completion_queue->Shutdown();
			handlers_.clear();

			logger_->info("{0} stopped", class_name());
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
			call->reader = stub_->AsyncGet(&call->context, message, it->second.completion_queue.get());
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));

			return utils::get_result(call->promise);
		}

		bool connect(const DataTypes::ConnectMsg& request) const
		{
			auto queue = utils::get_completion_queue<AsyncConnectCall>(handlers_);
			if (queue == nullptr)
				return false;

			auto call = new AsyncConnectCall;
			call->reader = stub_->AsyncConnect(&call->context, request, queue);
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));
						
			return utils::get_result(call->promise);
		}

		void heart_beat(const DataTypes::HeartbeatMessage& request) const
		{
			auto queue = utils::get_completion_queue<AsyncHeartbeatCall>(handlers_);
			if (queue == nullptr)
				return;

			auto call = new AsyncHeartbeatCall;
			call->reader = stub_->AsyncHeartbeat(&call->context, request, queue);
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));
		}

		void update_devices(const DataTypes::DeviceUpdate& request) const
		{
			auto queue = utils::get_completion_queue<AsyncUpdateDevicesCall>(handlers_);
			if (queue == nullptr)
				return;

			auto call = new AsyncUpdateDevicesCall;
			call->reader = stub_->AsyncUpdateDevices(&call->context, request, queue);
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));
		}
		
	private:
		template<typename T>
		void add_call_handler()
		{
			auto cq = std::make_shared<grpc::CompletionQueue>();

			auto callback = boost::bind(&CoordinatorClientImpl::async_complete_rpc<T>
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
					else
						logger_->error("{0} rpc failed", class_name());
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
		std::unique_ptr<Services::CoordinatorService::Stub> stub_;
		std::shared_ptr<grpc::Channel>                   channel_;

		ClientContext context_;
		ClientRequestHandlers handlers_;

		boost::asio::io_service io_service_;
		boost::thread_group threads_;

		std::shared_ptr<contracts::common::Logger> logger_;

		CoordinatorClientImpl(const CoordinatorClientImpl&) = delete;
		CoordinatorClientImpl& operator=(const CoordinatorClientImpl&) = delete;
	};
}

#endif