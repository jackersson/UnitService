#ifndef GetCardRequestHandler_Included
#define GetCardRequestHandler_Included

#include <memory>
#include <helpers/request_handler.hpp>
#include <iservice_context.hpp>
#include <services/unit_service.grpc.pb.h>

#include "unit_service_api.hpp"

namespace grpc_services
{
	namespace unit_service
	{		
		class GetCardRequestHandler 
			: public RequestHandler<Services::UnitService::AsyncService>		
		{
		public:
			GetCardRequestHandler
			( Services::UnitService::AsyncService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IServiceContext* context);
			
			void create_request_handler() override
			{
				new GetCardRequestHandler(service_, server_completion_queue_, context_);
			}

			void create_request() override
			{
				service_->RequestGetCard(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void process_request() override;

			static void create(Services::UnitService::AsyncService* service
				                , grpc::ServerCompletionQueue* completion_queue
				                , contracts::IServiceContext*     context)
			{
				new GetCardRequestHandler(service, completion_queue, context);
			}
		
		private:
			bool try_resolve_dependencies();

			void complete(const DataTypes::CardMsg& response);

			bool initialized_;
			DataTypes::Device  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::CardMsg> responder_;
			std::unique_ptr<GetCardApi> card_api_;
			contracts::IServiceContext* context_;
			//contracts::devices::access_device::IAccessDeviceEngine* engine_;

			const std::chrono::seconds REQUEST_TIMEOUT = std::chrono::seconds(3);				
		};
	}
}
#endif