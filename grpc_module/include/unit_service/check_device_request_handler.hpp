#ifndef CheckDeviceRequestHandler_Included
#define CheckDeviceRequestHandler_Included

#include <memory>

#include <helpers/request_handler.hpp>
#include <contracts/iunit_context.hpp>
#include <service_base.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		class CheckDeviceRequestHandler 
			           : public RequestHandler<services_api::AsyncUnitService>
		{
		public:
			CheckDeviceRequestHandler(services_api::AsyncUnitService* service
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
				new CheckDeviceRequestHandler(service_, server_completion_queue_, context_);
			}

			void CreateRequest() override
			{
				service_->RequestCheckDevice(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			static void Create(services_api::AsyncUnitService*            service
				                , grpc::ServerCompletionQueue* completion_queue
				                , contracts::IUnitContext*     context)
			{
				new CheckDeviceRequestHandler(service, completion_queue, context);
			}

			void ProcessRequest() override;	

		private:
			DataTypes::Device  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::CheckMsg> responder_;
			contracts::IUnitContext* context_;
		};
	}
}
#endif