#ifndef GetDevicesRequestHandler__INCLUDED
#define GetDevicesRequestHandler__INCLUDED
#include <memory>
#include <include/grpc++/impl/codegen/completion_queue.h>
#include <services/unit_service.grpc.pb.h>
#include "../request_handler.hpp"


namespace grpc_services
{
	namespace unit_service
	{/*
		typedef Services::UnitService::AsyncService UnitServicePtr;

		class GetDevicesRequestHandler : public RequestHandler<UnitServicePtr>
		{
		public:
			GetDevicesRequestHandler(std::shared_ptr<UnitServicePtr> service
				, grpc::ServerCompletionQueue* completion_queue)
				: responder_(&server_context_)
				,	RequestHandler<UnitServicePtr>(service, completion_queue )				
			{
				Proceed();
			}
		
			void CreateRequestHandler() override
			{
				new GetDevicesRequestHandler(service_, server_completion_queue_);
			}

			void CreateRequest() override
			{			
				service_->RequestGetDevices(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);			
			}

			void ProcessRequest() override
			{
				DataTypes::DevicesResponse response;

				responder_.Finish(response, grpc::Status::OK, this);
			}

		private:		
			DataTypes::GetDevicesRequest  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::DevicesResponse>    responder_;
			
		};
		*/
	}
	
}
#endif