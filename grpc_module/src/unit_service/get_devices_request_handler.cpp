#include "unit_service/get_devices_request_handler.hpp"

#include <helpers/request_adapters.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		void GetDevicesRequestHandler::process_request()
		{
			data_model::Devices devices;

			if (context_ != nullptr && context_->devices() != nullptr)
				context_->devices()->enumerate(devices);

			auto response = services_api::helpers::to_proto_devices(devices);
			responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}
