#ifndef CheckDeviceRequestHandler_Included
#define CheckDeviceRequestHandler_Included

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
		class CheckDeviceRequestHandler : public RequestHandler<AsyncService>
		{
		public:
			CheckDeviceRequestHandler(AsyncService* service
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
				new CheckDeviceRequestHandler(service_, server_completion_queue_, context_);
			}

			void CreateRequest() override
			{
				service_->RequestCheckDevice(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void ProcessRequest() override
			{
				DataTypes::CheckMsg response;
				response.set_ok(true); //TODO notify when something wrong

				auto access_device_engine = context_->devices()->access_device_engine();

				//TODO refactor
				auto device_name = request_.device_name();
				access_device_engine->add(device_name);
				access_device_engine->execute(device_name
					, contracts::devices::access_device::lights::lLight);

				access_device_engine->execute(device_name
					, contracts::devices::access_device::lights::lRedMain);

				access_device_engine->remove(device_name);

				std::cout << "Client wants check device" << std::endl;
				responder_.Finish(response, grpc::Status::OK, this);
			}

		private:
			DataTypes::Device  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::CheckMsg> responder_;
			contracts::IUnitContextPtr context_;
		};
	}
}
#endif