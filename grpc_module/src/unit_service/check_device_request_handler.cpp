#include <unit_service/check_device_request_handler.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		void CheckDeviceRequestHandler::ProcessRequest()
		{
			context_->logger()->info("Check device request");
			DataTypes::CheckMsg response;

			if (request_.device_type() == DataTypes::DeviceType::CardReader)
			{
				auto access_device_engine = context_->devices()->access_device_engine();
				bool result;
				try {
					result = access_device_engine->check(request_.device_name());
				}
				catch (std::exception& exception) {
					response.set_message(exception.what());
					result = false;
				}
				response.set_ok(result);
			}
			else
				response.set_message("Device type not valid. Should be Access Device");

			context_->logger()->info("Check device request done {0}", response.ok());
			responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}
