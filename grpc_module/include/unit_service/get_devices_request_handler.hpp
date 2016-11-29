#ifndef GetDevicesRequestHandler__INCLUDED
#define GetDevicesRequestHandler__INCLUDED

#include <memory>
#include <helpers/request_handler.hpp>
#include <iservice_context.hpp>
#include <services/unit_service.grpc.pb.h>

namespace grpc_services
{
	namespace unit_service
	{
		class GetDevicesRequestHandler 
			: public RequestHandler<Services::UnitService::AsyncService>
		{
		public:
			GetDevicesRequestHandler(Services::UnitService::AsyncService* service
				, grpc::ServerCompletionQueue*    completion_queue
				, contracts::IServiceContext*       context);
		
			void create_request_handler() override
			{
				new GetDevicesRequestHandler(service_, server_completion_queue_, context_);
			}

			void create_request() override
			{			
				service_->RequestGetDevices( &server_context_, &request_
					                         , &responder_     , server_completion_queue_
					                         , server_completion_queue_, this);			
			}

			void process_request() override;

			static void create(Services::UnitService::AsyncService* service
				                , grpc::ServerCompletionQueue* completion_queue
				                , contracts::IServiceContext*     context)
			{
				new GetDevicesRequestHandler(service, completion_queue, context);
			}

		private:	
			void complete(const data_model::Devices&);
			bool try_resolve_dependencies();

			bool initialized_;

			google::protobuf::Empty  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::Devices>    responder_;
			contracts::IServiceContext* context_;

		};
		
	}
	
}
#endif