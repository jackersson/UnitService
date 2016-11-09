#ifndef EnrollRequestHandler_Included
#define EnrollRequestHandler_Included

#include <memory>
#include <helpers/request_handler.hpp>
#include <contracts/iservice_context.hpp>
#include <async_service_base.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		class EnrollRequestHandler 
			: public RequestHandler<services_api::AsyncUnitService>
		{
		public:
			EnrollRequestHandler(services_api::AsyncUnitService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IServiceContext* context)
				: RequestHandler<services_api::AsyncUnitService>(service, completion_queue)
				, responder_(&server_context_)
				, context_(context)
			{
				Proceed();
			}

			void CreateRequestHandler() override
			{
				new EnrollRequestHandler(service_, server_completion_queue_, context_);
			}

			void CreateRequest() override
			{
				service_->RequestEnroll(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void ProcessRequest() override;	

		  static void Create( services_api::AsyncUnitService*            service
				                , grpc::ServerCompletionQueue* completion_queue
				                , contracts::IServiceContext*     context)
			{
				new EnrollRequestHandler(service, completion_queue, context);
			}

		private:
			DataTypes::Device  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::Faces> responder_;
			contracts::IServiceContext* context_;
		};
	}
}
#endif