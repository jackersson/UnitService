#ifndef FacialClientImpl_INCLUDED
#define FacialClientImpl_INCLUDED

#include <grpc++/grpc++.h>
#include <contracts/services/iservice.hpp>
#include "client_context.hpp"
#include <src/cpp/server/dynamic_thread_pool.h>
#include <services/coordinator_service.grpc.pb.h>

#include <service_utils.hpp>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <grpc_service_utils.hpp>

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
			threads_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
			logger_ = context.unit_context()->logger();
			FacialClientImpl::init();
		}

		~FacialClientImpl()	{
			FacialClientImpl::stop();
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
			

	private:
		template<typename T>
		void add_call_handler()
		{
			auto cq = std::make_shared<grpc::CompletionQueue>();

			auto callback = std::bind(&FacialClientImpl::async_complete_rpc<T>
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
			return typeid(FacialClientImpl).name();
		}

		bool active_;
		std::unique_ptr<Services::CoordinatorService::Stub> stub_;
		std::shared_ptr<grpc::Channel>                   channel_;

		ClientContext context_;
		ClientRequestHandlers handlers_;

		boost::asio::io_service io_service_;
		boost::thread_group threads_;

		std::shared_ptr<contracts::common::Logger> logger_;

		FacialClientImpl(const FacialClientImpl&) = delete;
		FacialClientImpl& operator=(const FacialClientImpl&) = delete;
	};
}

#endif