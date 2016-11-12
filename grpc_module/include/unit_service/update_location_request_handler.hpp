#ifndef UpdateLocationRequestHandler_Included
#define UpdateLocationRequestHandler_Included

#include <memory>
#include <helpers/request_handler.hpp>
#include <contracts/iservice_context.hpp>
#include <services/unit_service.grpc.pb.h>

namespace grpc_services
{
	namespace unit_service
	{
		class UpdateLocationRequestHandler 
			: public RequestHandler<Services::UnitService::AsyncService>
		{
		public:
			UpdateLocationRequestHandler(Services::UnitService::AsyncService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IServiceContext* context)
				: RequestHandler<Services::UnitService::AsyncService>(service, completion_queue)
				, responder_(&server_context_)
				, context_(context)
			{
				proceed();
			}

			void create_request_handler() override
			{
				new UpdateLocationRequestHandler(service_, server_completion_queue_, context_);
			}

			void create_request() override
			{
				service_->RequestUpdateLocation(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void process_request() override;

			static void create(Services::UnitService::AsyncService* service
				                , grpc::ServerCompletionQueue*        completion_queue
				                , contracts::IServiceContext*         context)
			{
				new UpdateLocationRequestHandler(service, completion_queue, context);
			}

		private:
			DataTypes::LocationUpdate  request_;
			grpc::ServerAsyncResponseWriter<google::protobuf::Empty> responder_;
			contracts::IServiceContext* context_;
		};
	}
}
#endif