#include <unit_service/get_devices_request_handler.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		void GetDevicesRequestHandler::ProcessRequest()
		{
			DataTypes::Devices response;

			if (context_ != nullptr && context_->devices() != nullptr)
				context_->devices()->enumerate(response);

			responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}