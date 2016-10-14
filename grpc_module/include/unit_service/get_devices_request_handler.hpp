#ifndef GetDevicesRequestHandler__INCLUDED
#define GetDevicesRequestHandler__INCLUDED
#include <memory>
#include <include/grpc++/impl/codegen/completion_queue.h>
#include <services/unit_service.grpc.pb.h>
#include "../request_handler.hpp"
#include <contracts/iunit_context.hpp>
#include <service_utils.hpp>

namespace grpc_services
{
	namespace unit_service
	{

		class GetDevicesRequestHandler : public RequestHandler<AsyncService>
		{
		public:
			GetDevicesRequestHandler(std::shared_ptr<AsyncService> service
				, grpc::ServerCompletionQueue* completion_queue
			  , contracts::IUnitContextPtr context)
				: RequestHandler<AsyncService>(service, completion_queue )
				,	responder_(&server_context_)			
				, context_(context)
			{
				Proceed();
			}
		
			void CreateRequestHandler() override
			{
				new GetDevicesRequestHandler(service_, server_completion_queue_, context_);
			}

			void CreateRequest() override
			{			
				service_->RequestGetDevices(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);			
			}

			void ProcessRequest() override
			{
				DataTypes::Devices response;

				//TODO test
				context_->devices()->enumerate(response);

				responder_.Finish(response, grpc::Status::OK, this);
			}

		private:		
			google::protobuf::Empty  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::Devices>    responder_;
			contracts::IUnitContextPtr context_;

		};
		
	}
	
}
#endif