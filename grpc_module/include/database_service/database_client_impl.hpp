#ifndef DatabaseClientImpl_INCLUDED
#define DatabaseClientImpl_INCLUDED

#include <grpc++/grpc++.h>
#include <contracts/services/iservice.hpp>
#include <services/database_service.grpc.pb.h>
#include "client_context.hpp"
#include <src/cpp/server/dynamic_thread_pool.h>
#include "database_service/database_client_calls.hpp"
#include <contracts/services/idatabase_api.hpp>
#include <service_utils.hpp>
#include <grpc_service_utils.hpp>

using grpc::ServerBuilder;

namespace grpc_services
{
	class DatabaseClientImpl : public contracts::services::IService
		, public contracts::services::IDatabaseApi
	{
	public:
		explicit DatabaseClientImpl(const ClientContext& context)
			: active_(false), context_(context)
			, thread_pool_(0)
		{
			auto unit_context = context.unit_context();
			if (unit_context != nullptr)
				logger_ = unit_context->logger();
			else
				logger_ = std::shared_ptr<contracts::common::Logger>();
			DatabaseClientImpl::init();
		}

		~DatabaseClientImpl() {
			DatabaseClientImpl::stop();
			logger_->info("{0} destroyed", class_name());
		}

		void init() override
		{
			auto address = context_.address().get();
			logger_->info("Try create channel {0}", address);

			channel_ = CreateChannel(address, grpc::InsecureChannelCredentials());
			stub_ = Services::DatabaseService::NewStub(channel_);

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

		std::shared_ptr<DataTypes::GetResponse>
			get(const DataTypes::GetRequest& request) override
		{
			logger_->info("{0} Get request ", class_name());

			DataTypes::MessageBytes message;
			helpers::to_bytes(request, message);

			auto queue = helpers::get_completion_queue<AsyncGetRequestCall>(handlers_);
			if (queue == nullptr)
				return nullptr;

			auto call = new AsyncGetRequestCall;
			call->reader = stub_->AsyncGet(&call->context, message, queue);
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));

			try
			{
				auto result = utils::service::get_result(call->promise, std::chrono::milliseconds(300));
				return result;
			}
			catch (std::exception&) {
				return nullptr;
			}
		}

		std::shared_ptr<DataTypes::CommitResponse>
			commit(const DataTypes::CommitRequest& request) override
		{
			DataTypes::MessageBytes message;
			helpers::to_bytes(request, message);

			auto it = handlers_.find(typeid(AsyncCommitRequestCall).name());
			if (it == handlers_.end())
				return nullptr;

			auto call = new AsyncCommitRequestCall;
			call->reader = stub_->AsyncCommit(&call->context, message, it->second.completion_queue.get());
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));

			try
			{
				auto result = utils::service::get_result(call->promise, std::chrono::milliseconds(300));
				return result;
			}
			catch (std::exception&) {
				return nullptr;
			}
		}


	private:
		template<typename T>
		void add_call_handler()
		{
			auto cq = std::make_shared<grpc::CompletionQueue>();

			auto callback = std::bind(&DatabaseClientImpl::async_complete_rpc<T>
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
					logger_->error("{0} rpc failed. {1}", class_name(), ex.what());
				}
				delete call;
			}
		}

		std::string class_name() const {
			return typeid(DatabaseClientImpl).name();
		}

		bool active_;
		std::unique_ptr<Services::DatabaseService::Stub> stub_;
		std::shared_ptr<grpc::Channel>                   channel_;

		ClientContext context_;
		ClientRequestHandlers handlers_;

		std::shared_ptr<contracts::common::Logger> logger_;

		grpc::DynamicThreadPool thread_pool_;

		DatabaseClientImpl(const DatabaseClientImpl&) = delete;
		DatabaseClientImpl& operator=(const DatabaseClientImpl&) = delete;
	};
}

#endif