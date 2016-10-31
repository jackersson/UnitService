#ifndef UpdateLocationRequestHandler_Included
#define UpdateLocationRequestHandler_Included

#include <memory>
#include <request_handler.hpp>
#include <contracts/iunit_context.hpp>
#include <service_base.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		class UpdateLocationRequestHandler : public RequestHandler<services_api::AsyncUnitService>
		{
		public:
			UpdateLocationRequestHandler(services_api::AsyncUnitService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IUnitContext* context)
				: RequestHandler<services_api::AsyncUnitService>(service, completion_queue)
				, responder_(&server_context_)
				, context_(context)
			{
				Proceed();
			}

			void CreateRequestHandler() override
			{
				new UpdateLocationRequestHandler(service_, server_completion_queue_, context_);
			}

			void CreateRequest() override
			{
				service_->RequestUpdateLocation(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void ProcessRequest() override;	

			static void Create(services_api::AsyncUnitService*            service
				                , grpc::ServerCompletionQueue* completion_queue
				                , contracts::IUnitContext*     context)
			{
				new UpdateLocationRequestHandler(service, completion_queue, context);
			}

		private:
			DataTypes::Location  request_;
			grpc::ServerAsyncResponseWriter<google::protobuf::Empty> responder_;
			contracts::IUnitContext* context_;
		};
	}
}
#endif