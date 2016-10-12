#ifndef OpenDoorRequestHandler_Included
#define OpenDoorRequestHandler_Included

#include <memory>
#include <include/grpc++/impl/codegen/completion_queue.h>
#include <services/unit_service.grpc.pb.h>
#include <request_handler.hpp>

namespace grpc_services
{
	namespace unit_service
	{		
		typedef Services::UnitService::AsyncService UnitServicePtr;
		
		class OpenDoorRequestHandler : public RequestHandler<UnitServicePtr>
		{
		public:
			OpenDoorRequestHandler( std::shared_ptr<UnitServicePtr> service
	                        	, grpc::ServerCompletionQueue* completion_queue)
	                        	: RequestHandler<UnitServicePtr>(service, completion_queue )
		                        ,	responder_(&server_context_)
		{
	  	Proceed();
		}

		void CreateRequestHandler() override
		{
			new OpenDoorRequestHandler(service_, server_completion_queue_);
		}

		void CreateRequest() override
		{
			service_->RequestOpenDoor(&server_context_, &request_
				, &responder_, server_completion_queue_
				, server_completion_queue_, this);
		}

		void ProcessRequest() override
		{
			google::protobuf::Empty response;
			std::cout << "Client wants open door" << std::endl;
			responder_.Finish(response, grpc::Status::OK, this);
		}

		private:
			DataTypes::Location  request_;
			grpc::ServerAsyncResponseWriter<google::protobuf::Empty>    responder_;

		};		
	}
}
#endif