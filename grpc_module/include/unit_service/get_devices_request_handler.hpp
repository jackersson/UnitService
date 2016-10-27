#ifndef GetDevicesRequestHandler__INCLUDED
#define GetDevicesRequestHandler__INCLUDED
#include <memory>
#include <include/grpc++/impl/codegen/completion_queue.h>
#include <services/unit_service.grpc.pb.h>
#include "request_handler.hpp"
#include <contracts/iunit_context.hpp>
#include "unit_service_impl.hpp"

namespace grpc_services
{
	namespace unit_service
	{

		class GetDevicesRequestHandler : public RequestHandler<AsyncService>
		{
		public:
			GetDevicesRequestHandler(AsyncService* service
				, grpc::ServerCompletionQueue* completion_queue
			  , contracts::IUnitContext* context)
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

			void ProcessRequest() override;		

		private:		
			google::protobuf::Empty  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::Devices>    responder_;
			contracts::IUnitContext* context_;

		};
		
	}
	
}
#endif