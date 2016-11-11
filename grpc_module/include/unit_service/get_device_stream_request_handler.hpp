#ifndef GetDeviceStreamRequestHandler_Included
#define GetDeviceStreamRequestHandler_Included

#include <memory>
#include <helpers/request_handler.hpp>
#include <contracts/iservice_context.hpp>
#include <async_service_base.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		class GetDeviceStreamRequestHandler : public RequestHandler<services_api::AsyncUnitService>
		{
		public:
			GetDeviceStreamRequestHandler(services_api::AsyncUnitService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IServiceContext* context)
				: RequestHandler<services_api::AsyncUnitService>(service, completion_queue)
				, responder_(&server_context_)
				, context_(context)
			{
				proceed();
			}

			void create_request_handler() override
			{
				new GetDeviceStreamRequestHandler(service_, server_completion_queue_, context_);
			}

			void create_request() override
			{
				service_->RequestGetDeviceStream(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void process_request() override;

			static void create(services_api::AsyncUnitService*            service
				                , grpc::ServerCompletionQueue* completion_queue
				                , contracts::IServiceContext*     context)
			{
				new GetDeviceStreamRequestHandler(service, completion_queue, context);
			}

		private:
			DataTypes::Device  request_;
			grpc::ServerAsyncWriter<DataTypes::FrameBytes> responder_;
			contracts::IServiceContext* context_;
		};
	}
}
#endif