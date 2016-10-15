#ifndef UnitServiceImpl_INCLUDED
#define UnitServiceImpl_INCLUDED

#include <services/unit_service.grpc.pb.h>
#include <contracts/services/iservice.hpp>
#include <list>
#include "server_context.hpp"
#include <contracts/services/service_address.hpp>
#include "unit_service/open_door_request_handler.hpp"
#include <service_utils.hpp>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

using grpc::ServerBuilder;

namespace grpc_services
{
	class UnitServiceImpl : public contracts::services::IService
	{
	public:
		explicit UnitServiceImpl(ServerContext& context) : context_(context)
		{
			threads_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
			logger_ = context.unit_context()->logger();
			UnitServiceImpl::init();
		}

		virtual ~UnitServiceImpl()
		{
			UnitServiceImpl::stop();
			logger_->info("{0} destroyed", class_name());
		}

		void init() override
		{
			auto address = context_.address().get();
			logger_->info("Try open port on {0}", address);

			auto builder = context_.server_builder();
			builder->AddListeningPort( address, grpc::InsecureServerCredentials());
						
			builder->RegisterService(&service_);

			add_rpc_handler<unit_service::OpenDoorRequestHandler>(builder);
		}

		void start() override
		{
			for (auto handler: handlers_)
				io_service_.post(handler.callback);
	
			logger_->info( "{0} listening on {1}", class_name()
				           , context_.address().get() );
		}

		void stop() override
		{
			io_service_.stop();	
			for (auto& it : handlers_)
				it.completion_queue->Shutdown();
			handlers_.clear();

			logger_->info("{0} stopped", class_name());
		}

	private:
		template<typename T>
		void handle_rpc(grpc::ServerCompletionQueue* queue) 
		{
			new T(&service_, queue, context_.unit_context());
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
					logger_->error(ex.what());
				}
			}
		}

		template<typename T>
		void add_rpc_handler(std::shared_ptr<grpc::ServerBuilder> builder)
		{
			auto cq_ = builder->AddCompletionQueue();			
			auto callback = boost::bind(&UnitServiceImpl::handle_rpc<T>, this, cq_.get());
			handlers_.push_back(ServerRequestHandler(move(cq_), callback));
		}

		std::string class_name() const
		{
			return typeid(UnitServiceImpl).name();
		}

		UnitServiceImpl(const UnitServiceImpl&) = delete;
		UnitServiceImpl& operator=(const UnitServiceImpl&) = delete;

	  AsyncService  service_;

		boost::asio::io_service io_service_;
		boost::thread_group threads_;
		
		ServerRequestHandlers handlers_;
		ServerContext context_;
		std::shared_ptr<contracts::common::Logger> logger_;
	};
}

#endif