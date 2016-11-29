#ifndef EnrollRequestHandler_Included
#define EnrollRequestHandler_Included

#include <memory>
#include <helpers/request_handler.hpp>
#include <iservice_context.hpp>
#include <services/unit_service.grpc.pb.h>

namespace grpc_services
{
	namespace unit_service
	{
		class EnrollRequestHandler 
			: public RequestHandler<Services::UnitService::AsyncService>
		{
		public:
			EnrollRequestHandler( Services::UnitService::AsyncService* service
				                  , grpc::ServerCompletionQueue* completion_queue
				                  , contracts::IServiceContext* context);

			void create_request_handler() override
			{
				new EnrollRequestHandler(service_, server_completion_queue_, context_);
			}

			void create_request() override
			{
				service_->RequestEnroll(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void process_request() override;

		  static void create(Services::UnitService::AsyncService* service
				                , grpc::ServerCompletionQueue* completion_queue
				                , contracts::IServiceContext*     context)
			{
				new EnrollRequestHandler(service, completion_queue, context);
			}

		private:
			bool initialized_;

			DataTypes::Device  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::Faces> responder_;
			contracts::IServiceContext* context_;
		};
	}
}
#endif