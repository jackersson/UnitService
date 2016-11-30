/*#ifndef CheckDeviceRequestHandler_Included
#define CheckDeviceRequestHandler_Included

#include <memory>
#include <helpers/request_handler.hpp>
#include <iservice_context.hpp>
#include <services/unit_service.grpc.pb.h>

#include "unit_service_api.hpp"

namespace grpc_services
{
	namespace unit_service
	{
		class CheckDeviceRequestHandler 
			: public RequestHandler<Services::UnitService::AsyncService>			           
		{
		public:
			CheckDeviceRequestHandler
			 ( Services::UnitService::AsyncService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IServiceContext* context);

			~CheckDeviceRequestHandler();

			void create_request_handler() override
			{
				create(service_, server_completion_queue_, context_);
				//new CheckDeviceRequestHandler(service_, server_completion_queue_, context_);
			}

			void create_request() override
			{
				service_->RequestCheckDevice(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			static void create(Services::UnitService::AsyncService*  service
				                , grpc::ServerCompletionQueue* completion_queue
				                , contracts::IServiceContext*     context)
			{
				new CheckDeviceRequestHandler(service, completion_queue, context);
			}

			void process_request() override;

		private:
			bool try_resolve_dependencies();

			void complete(const DataTypes::CheckMsg& response);

			bool initialized_;
			DataTypes::Device  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::CheckMsg> responder_;
			contracts::IServiceContext* context_;
			access_device::IAccessDeviceEngine* engine_;

			const std::chrono::seconds REQUEST_TIMEOUT = std::chrono::seconds(3);
		};
	}
}
#endif
*/