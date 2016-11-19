#ifndef OpenDoorRequestHandler_Included
#define OpenDoorRequestHandler_Included


#include <helpers/request_handler.hpp>
#include <contracts/iservice_context.hpp>
#include <services/unit_service.grpc.pb.h>

namespace grpc_services
{
	namespace unit_service
	{		
		class OpenDoorRequestHandler 
			: public RequestHandler<Services::UnitService::AsyncService>
		{
		public:
			OpenDoorRequestHandler(Services::UnitService::AsyncService* service
	      , grpc::ServerCompletionQueue* completion_queue
			  , contracts::IServiceContext* context )
	      : RequestHandler<Services::UnitService::AsyncService>(service, completion_queue )
		    ,	responder_(&server_context_)
			  , context_(context)
		{
			if (context_ == nullptr)
				throw std::exception("Context can't be null");
			if (context_->track_locations() == nullptr)
				throw std::exception("Track locations can't be null");

			proceed();
		}

		void create_request_handler() override
		{
			new OpenDoorRequestHandler(service_, server_completion_queue_, context_);
		}

		void create_request() override
		{
			service_->RequestOpenDoor(&server_context_, &request_
				, &responder_, server_completion_queue_
				, server_completion_queue_, this);
		}

		void process_request() override;

		 static void create( Services::UnitService::AsyncService* service
			                 , grpc::ServerCompletionQueue* completion_queue
				               , contracts::IServiceContext*     context)
			{
				new OpenDoorRequestHandler(service, completion_queue, context);
			}

		private:
			DataTypes::Location  request_;
			grpc::ServerAsyncResponseWriter<google::protobuf::Empty> responder_;
			contracts::IServiceContext* context_;

			const std::chrono::seconds REQUEST_TIMEOUT = std::chrono::seconds(3);
		};		
	}
}
#endif