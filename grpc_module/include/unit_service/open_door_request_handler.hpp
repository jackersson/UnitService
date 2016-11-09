#ifndef OpenDoorRequestHandler_Included
#define OpenDoorRequestHandler_Included


#include <helpers/request_handler.hpp>
#include <contracts/iservice_context.hpp>
#include <async_service_base.hpp>

namespace grpc_services
{
	namespace unit_service
	{		
		class OpenDoorRequestHandler : public RequestHandler<services_api::AsyncUnitService>
		{
		public:
			OpenDoorRequestHandler(services_api::AsyncUnitService* service
	                        	, grpc::ServerCompletionQueue* completion_queue
			                      , contracts::IServiceContext* context )
	                        	: RequestHandler<services_api::AsyncUnitService>(service, completion_queue )
		                        ,	responder_(&server_context_)
			                     	, context_(context)
		{
	  	Proceed();
		}

		void CreateRequestHandler() override
		{
			new OpenDoorRequestHandler(service_, server_completion_queue_, context_);
		}

		void CreateRequest() override
		{
			service_->RequestOpenDoor(&server_context_, &request_
				, &responder_, server_completion_queue_
				, server_completion_queue_, this);
		}

		void ProcessRequest() override;		

		 static void Create(services_api::AsyncUnitService*            service
				               , grpc::ServerCompletionQueue* completion_queue
				               , contracts::IServiceContext*     context)
			{
				new OpenDoorRequestHandler(service, completion_queue, context);
			}

		private:
			DataTypes::Location  request_;
			grpc::ServerAsyncResponseWriter<google::protobuf::Empty>    responder_;
			contracts::IServiceContext* context_;
		};		
	}
}
#endif