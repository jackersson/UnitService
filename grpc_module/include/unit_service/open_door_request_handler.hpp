#ifndef OpenDoorRequestHandler_Included
#define OpenDoorRequestHandler_Included

#include <include/grpc++/impl/codegen/completion_queue.h>
#include <services/unit_service.grpc.pb.h>
#include <request_handler.hpp>
#include <contracts/iunit_context.hpp>
#include <grpc_service_utils.hpp>

namespace grpc_services
{
	namespace unit_service
	{		
		class OpenDoorRequestHandler : public RequestHandler<AsyncService>
		{
		public:
			OpenDoorRequestHandler( AsyncService* service
	                        	, grpc::ServerCompletionQueue* completion_queue
			                      , contracts::IUnitContext* context )
	                        	: RequestHandler<AsyncService>(service, completion_queue )
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

		private:
			DataTypes::Location  request_;
			grpc::ServerAsyncResponseWriter<google::protobuf::Empty>    responder_;
			contracts::IUnitContext* context_;
		};		
	}
}
#endif