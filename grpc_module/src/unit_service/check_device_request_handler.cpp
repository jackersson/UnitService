#include "unit_service/check_device_request_handler.hpp"

#include <datatypes/devices.pb.h>
#include <data/models/devices.hpp>
#include <future>

using namespace contracts::devices;

namespace grpc_services
{
	namespace unit_service
	{
		void CheckDeviceRequestHandler::process_request()
		{
			logger_.info("Check device request");
			DataTypes::CheckMsg response;

			if (request_.device_type() == DataTypes::DeviceType::CardReader)
			{				
				auto serial = static_cast<uint16_t>(request_.serial_number());
				engine_->add(data_model::DeviceId("any", serial));
			}
			else
			{
				response.set_message("Device type not valid. Should be Access Device");
				complete(response);
			}			
		}
		
		void CheckDeviceRequestHandler::complete(const DataTypes::CheckMsg& response)
		{			
			auto serial = static_cast<uint16_t>(request_.serial_number());
			engine_->remove(data_model::DeviceId("any", serial));

			logger_.info("Check device request done {0}", response.ok());
			responder_.Finish(response, grpc::Status::OK, this);
			next();
		}

		void CheckDeviceRequestHandler::wait_for_device()
		{
			auto handle = std::async(std::launch::async,
				[this]()
			{
				std::this_thread::sleep_for(REQUEST_TIMEOUT);
				if (this == nullptr || this->status_ != RequestStatus::FINISH)
				{
					DataTypes::CheckMsg response;
					auto serial = static_cast<uint16_t>(request_.serial_number());
					if (!engine_->is_active(data_model::DeviceId(serial)))
					{
						response.set_ok(false);
						response.set_message("Failed to connect to device");
					}
					else 
						response.set_ok(true);

					complete(response);
				}
			});
		}

	}
}
