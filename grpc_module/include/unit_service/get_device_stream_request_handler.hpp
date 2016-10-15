#ifndef GetDeviceStreamRequestHandler_Included
#define GetDeviceStreamRequestHandler_Included

#include <memory>
#include <include/grpc++/impl/codegen/completion_queue.h>
#include <services/unit_service.grpc.pb.h>
#include <request_handler.hpp>
#include <contracts/iunit_context.hpp>
#include <service_utils.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		

		class GetDeviceStreamRequestHandler : public RequestHandler<AsyncService>
		{
		public:
			GetDeviceStreamRequestHandler(AsyncService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IUnitContextPtr context)
				: RequestHandler<AsyncService>(service, completion_queue)
				, responder_(&server_context_)
				, context_(context)
			{
				Proceed();
			}

			void CreateRequestHandler() override
			{
				new GetDeviceStreamRequestHandler(service_, server_completion_queue_, context_);
			}

			void CreateRequest() override
			{
				service_->RequestGetDeviceStream(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void ProcessRequest() override
			{
				google::protobuf::Empty response;
				//TODO handle frames
				//context_->track_locations()->grant_access(request_);

				std::cout << "Client wants capture device video stream" << std::endl;
				//responder_.Finish(response, grpc::Status::OK, this);
			}

		private:
			DataTypes::Device  request_;
			grpc::ServerAsyncWriter<DataTypes::FrameBytes> responder_;
			contracts::IUnitContextPtr context_;
		};
	}
}
#endif