#ifndef GetCardRequestHandler_Included
#define GetCardRequestHandler_Included

#include <memory>
#include <helpers/request_handler.hpp>
#include <contracts/iservice_context.hpp>
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
			GetCardRequestHandler(Services::UnitService::AsyncService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IServiceContext* context)
				: RequestHandler<Services::UnitService::AsyncService>(service, completion_queue)
				, responder_(&server_context_)
				, context_(context)			
			{
				if (context_ == nullptr)
					throw std::exception("Context can't be null");
				auto devices = context_->devices();
				if (devices == nullptr)
					throw std::exception("Devices can't be null");

				auto engine_ = devices->access_device_engine();
				if (engine_ == nullptr)
					throw std::exception("Access Device engine can't be null");

				card_api_ = std::make_unique<GetCardApi>(engine_);				
				proceed();
			}
			
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
			void complete(const DataTypes::CardMsg& response);

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