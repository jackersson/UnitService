#ifndef FacialClientImpl_INCLUDED
#define FacialClientImpl_INCLUDED

#include <grpc++/grpc++.h>
#include <contracts/services/iservice.hpp>
#include "client_context.hpp"
#include <src/cpp/server/dynamic_thread_pool.h>
#include <services/coordinator_service.grpc.pb.h>

#include <service_utils.hpp>

using grpc::ServerBuilder;

namespace grpc_services
{
	class FacialClientImpl : public contracts::services::IService
		, public contracts::services::IDatabaseApi
	{
	public:
		explicit FacialClientImpl(const ClientContext& context)
			: active_(false), context_(context)
		{
			FacialClientImpl::init();
		}

		~FacialClientImpl()
		{
			FacialClientImpl::stop();
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
			

	private:
		template<typename T>
		void add_call_handler()
		{
			auto cq = std::make_shared<grpc::CompletionQueue>();

			auto callback = std::bind(&FacialClientImpl::async_complete_rpc<T>
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

		FacialClientImpl(const FacialClientImpl&) = delete;
		FacialClientImpl& operator=(const FacialClientImpl&) = delete;
	};
}

#endif