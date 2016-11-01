#ifndef GetCardRequestHandler_Included
#define GetCardRequestHandler_Included

#include <memory>
#include <helpers/request_handler.hpp>
#include <contracts/iunit_context.hpp>
#include <service_base.hpp>

namespace grpc_services
{
	namespace unit_service
	{		
		class GetCardRequestHandler : public RequestHandler<services_api::AsyncUnitService>
		{
		public:
			GetCardRequestHandler(services_api::AsyncUnitService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IUnitContext* context)
				: RequestHandler<services_api::AsyncUnitService>(service, completion_queue)
				, responder_(&server_context_)
				, context_(context)
				, creation_time_(clock())
			{
				Proceed();
			}
			
			void CreateRequestHandler() override
			{
				new GetCardRequestHandler(service_, server_completion_queue_, context_);
			}

			void CreateRequest() override
			{
				service_->RequestGetCard(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void ProcessRequest() override;

			static void Create(services_api::AsyncUnitService*            service
				                , grpc::ServerCompletionQueue* completion_queue
				                , contracts::IUnitContext*     context)
			{
				new GetCardRequestHandler(service, completion_queue, context);
			}

		private:
			DataTypes::Device  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::CardMsg> responder_;
			contracts::IUnitContext* context_;
			clock_t creation_time_;
		};
	}
}
#endif