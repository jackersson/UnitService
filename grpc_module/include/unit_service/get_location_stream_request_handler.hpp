#ifndef GetLocationStreamRequestHandler_Included
#define GetLocationStreamRequestHandler_Included

#include <helpers/request_handler.hpp>
#include <contracts/iservice_context.hpp>
#include <services/unit_service.grpc.pb.h>

namespace grpc_services
{
	namespace unit_service
	{	
		class GetLocationStreamRequestHandler 
			: public RequestHandler<Services::UnitService::AsyncService>
		{
		public:
			GetLocationStreamRequestHandler(Services::UnitService::AsyncService* service
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
				new GetLocationStreamRequestHandler(service_, server_completion_queue_, context_);
			}

			void create_request() override
			{
				service_->RequestGetLocationStream(&server_context_
					                , &responder_, server_completion_queue_
					                , server_completion_queue_, this);
			}

			void process_request() override;

			
			static void create(Services::UnitService::AsyncService* service
				                , grpc::ServerCompletionQueue* completion_queue
				                , contracts::IServiceContext*     context)
			{
				new GetLocationStreamRequestHandler(service, completion_queue, context);
			}

		private:
			DataTypes::Location  request_;
			grpc::ServerAsyncReaderWriter<DataTypes::FrameBytes, Services::StreamMsg > responder_;
			contracts::IServiceContext* context_;
		};
	}
}
#endif