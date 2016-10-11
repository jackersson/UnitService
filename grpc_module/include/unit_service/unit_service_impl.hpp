#ifndef UnitServiceImpl_INCLUDED
#define UnitServiceImpl_INCLUDED

#include <services/unit_service.grpc.pb.h>
#include <contracts/services/iservice.hpp>
#include <src/cpp/server/dynamic_thread_pool.h>
#include <list>
#include "service_context.hpp"
#include <contracts/services/service_address.hpp>
#include "unit_service/open_door_request_handler.hpp"

using grpc::ServerBuilder;

namespace grpc_services
{
	class UnitServiceImpl : public contracts::services::IService
	{
	public:

		typedef std::function<void()> RpcCallbackFunction;
		typedef std::pair<std::shared_ptr<grpc::ServerCompletionQueue>
			, RpcCallbackFunction> RequestHandler;
		typedef std::list<RequestHandler> RequestHandlers;

		explicit UnitServiceImpl(ServiceContext& context) : context_(context)
		{
			thread_pool_ = std::make_shared<grpc::DynamicThreadPool>(MAX_THREAD_POOL_CAPACITY);
			UnitServiceImpl::init();
		}

		virtual ~UnitServiceImpl()
		{
			UnitServiceImpl::stop();
		}

		void init() override
		{
			auto builder = context_.ServerBuilder();
			builder->AddListeningPort(context_.Address().FormattedAddress()
				, grpc::InsecureServerCredentials());

			service_ = std::make_shared<Services::UnitService::AsyncService>();
			builder->RegisterService(service_.get());

			//AddRpcHandler<unit_service::GetDevicesRequestHandler>(*thread_pool_, builder);
			AddRpcHandler<unit_service::OpenDoorRequestHandler>(builder);
			//AddRpcHandler<unit_service::CreatePopulationHandler>(builder);
		}

		void start() override
		{
			for (auto it = handlers_.begin(); it != handlers_.end(); ++it)
				thread_pool_->Add(it->second);

			std::cout << "Server listening on " << context_.Address().FormattedAddress() << std::endl;
		}

		void stop() override
		{
			for (auto it : handlers_)
				it.first->Shutdown();
			handlers_.clear();
		}

	private:
		template<typename T>
		void HandleRpc(grpc::ServerCompletionQueue* queue) const
		{
			new T(service_, queue);
			void* tag;
			bool  ok;

			while (true)
			{
				try
				{
					queue->Next(&tag, &ok);
					if (ok)
						static_cast<T*>(tag)->Proceed();
				}
				catch (std::exception& ex) {
					//TODO log
					std::cout << ex.what() << std::endl;
				}
			}
		}

		template<typename T>
		void AddRpcHandler(std::shared_ptr<grpc::ServerBuilder> builder)
		{
			std::shared_ptr<grpc::ServerCompletionQueue> cq_(builder->AddCompletionQueue());

			auto callback = std::bind(&UnitServiceImpl::HandleRpc<T>, this, cq_.get());
			handlers_.push_back(RequestHandler(cq_, callback));
		}

		UnitServiceImpl(const UnitServiceImpl&) = delete;
		UnitServiceImpl& operator=(const UnitServiceImpl&) = delete;


		std::shared_ptr<Services::UnitService::AsyncService>  service_;
		std::shared_ptr<grpc::ThreadPoolInterface>            thread_pool_;

		const int MAX_THREAD_POOL_CAPACITY = 10;

		RequestHandlers handlers_;
		ServiceContext context_;
	};
}

#endif