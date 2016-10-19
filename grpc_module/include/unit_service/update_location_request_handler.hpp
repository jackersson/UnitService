#ifndef UpdateLocationRequestHandler_Included
#define UpdateLocationRequestHandler_Included

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
		class UpdateLocationRequestHandler : public RequestHandler<AsyncService>
		{
		public:
			UpdateLocationRequestHandler(AsyncService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IUnitContext* context)
				: RequestHandler<AsyncService>(service, completion_queue)
				, responder_(&server_context_)
				, context_(context)
			{
				Proceed();
			}

			void CreateRequestHandler() override
			{
				new UpdateLocationRequestHandler(service_, server_completion_queue_, context_);
			}

			void CreateRequest() override
			{
				service_->RequestUpdateLocation(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void ProcessRequest() override
			{
				google::protobuf::Empty response;

				//TODO update location in local storage and coordinator
				context_->repository()->locations()->local()->update(&request_);

				std::cout << "Coordinator wants update location" << std::endl;
				responder_.Finish(response, grpc::Status::OK, this);
			}

		private:
			DataTypes::Location  request_;
			grpc::ServerAsyncResponseWriter<google::protobuf::Empty> responder_;
			contracts::IUnitContext* context_;
		};
	}
}
#endif