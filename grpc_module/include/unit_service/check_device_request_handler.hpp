#ifndef CheckDeviceRequestHandler_Included
#define CheckDeviceRequestHandler_Included

#include <memory>

#include <helpers/request_handler.hpp>
#include <contracts/iservice_context.hpp>
#include <async_service_base.hpp>

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
				                       , contracts::IServiceContext* context)
				                       : RequestHandler<services_api::AsyncUnitService>(service, completion_queue)
				                       , responder_(&server_context_)
				                       , context_(context)
			{
				if  (context_ == nullptr)
					throw std::exception("Context can't be null");
				auto devices = context_->devices();
				if (devices == nullptr)
					throw std::exception("Devices can't be null");

				engine_ = devices->access_device_engine();
				if (devices == nullptr)
					throw std::exception("Access Device engine can't be null");

				proceed();
			}

			void create_request_handler() override
			{
				new CheckDeviceRequestHandler(service_, server_completion_queue_, context_);
			}

			void create_request() override
			{
				service_->RequestCheckDevice(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			static void create(services_api::AsyncUnitService*            service
				                , grpc::ServerCompletionQueue* completion_queue
				                , contracts::IServiceContext*     context)
			{
				new CheckDeviceRequestHandler(service, completion_queue, context);
			}

			void process_request() override;


		private:
			void complete(const DataTypes::CheckMsg& response);
			void wait_for_device();

			DataTypes::Device  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::CheckMsg> responder_;
			contracts::IServiceContext* context_;
			contracts::devices::access_device::IAccessDeviceEngine* engine_;

			const std::chrono::seconds REQUEST_TIMEOUT = std::chrono::seconds(3);
		};
	}
}
#endif