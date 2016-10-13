#ifndef CoordinatorClientImpl_INCLUDED
#define CoordinatorClientImpl_INCLUDED

#include <grpc++/grpc++.h>
#include <contracts/services/iservice.hpp>
#include <services/database_service.grpc.pb.h>
#include "client_context.hpp"
#include <src/cpp/server/dynamic_thread_pool.h>
#include <services/coordinator_service.grpc.pb.h>
#include <database_service/database_client_calls.hpp>
#include "coordinator_client_calls.hpp"

#include <service_utils.hpp>

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
			CoordinatorClientImpl::init();
		}

		~CoordinatorClientImpl()
		{
			CoordinatorClientImpl::stop();
		}

		void init() override
		{
			channel_ = grpc::CreateChannel(context_.Address().FormattedAddress()
				, grpc::InsecureChannelCredentials());
			thread_pool_ = std::make_shared<grpc::DynamicThreadPool>(MAX_THREAD_POOL_CAPACITY);
			stub_ = Services::CoordinatorService::NewStub(channel_);

			//add_call_handler<AsyncGetRequestCall>();
			//add_call_handler<AsyncCommitRequestCall>();
		}

		void start() override
		{
			if (active_)
				return;

			active_ = true;

			for (auto it = handlers_.begin(); it != handlers_.end(); ++it)
				thread_pool_->Add(it->second.second);

			std::cout << "Client connected to " << context_.Address().FormattedAddress() << std::endl;
		}

		void stop() override
		{
			for (auto it : handlers_)
				it.second.first->Shutdown();
			handlers_.clear();
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
			call->reader = stub_->AsyncGet(&call->context, message, it->second.first.get());
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

			auto callback = std::bind(&CoordinatorClientImpl::async_complete_rpc<T>
				, this, cq.get());
			handlers_.insert(
				std::pair<std::string, CallHandler>(
					typeid(T).name(), CallHandler(cq, callback)));
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
					{
						call->parsed_response();
						std::cout << "Received" << std::endl;
					}
					else
						std::cout << "Rpc failed" << std::endl;
				}
				catch (std::exception& ex) {
					std::cout << ex.what() << std::endl;
				}
				delete call;
			}
		}

		bool active_;
		std::shared_ptr<grpc::ThreadPoolInterface>       thread_pool_;
		std::unique_ptr<Services::CoordinatorService::Stub> stub_;
		std::shared_ptr<grpc::Channel>                   channel_;

		ClientContext context_;
		CallHandlers handlers_;

		const int MAX_THREAD_POOL_CAPACITY = 10;

		CoordinatorClientImpl(const CoordinatorClientImpl&) = delete;
		CoordinatorClientImpl& operator=(const CoordinatorClientImpl&) = delete;
	};
}

#endif