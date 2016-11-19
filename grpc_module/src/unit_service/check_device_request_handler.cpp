#include "unit_service/check_device_request_handler.hpp"

#include <datatypes/devices.pb.h>
#include <data/models/devices.hpp>
#include <boost/thread/future.hpp>

using namespace contracts::devices;

namespace grpc_services
{
	namespace unit_service
	{
		void CheckDeviceRequestHandler::process_request()
		{
			logger_.info("Check device request -> in");

			if (request_.device_type() == DataTypes::DeviceType::CardReader)
			{
				auto serial = static_cast<uint16_t>(request_.serial_number());
				data_model::DeviceId dev("any", serial);

				boost::async([this, dev]() {
					return grpc_services::unit_service::check_device(engine_->enumerator(), dev);
				})
					.then([this](boost::future<bool> f) {
					DataTypes::CheckMsg response;
					auto ok = f.get();
					response.set_ok(ok);
					if (!ok)
						response.set_message("Failed to connect to device");
					complete(response);
				});
			}
			else
			{
				logger_.info("Check device request error: Device type not valid");
				DataTypes::CheckMsg response;
				response.set_ok(false);
				response.set_message("Device type not valid. Should be Access Device");
				complete(response);
			}
		}
		
		void CheckDeviceRequestHandler::complete(const DataTypes::CheckMsg& response)
		{			
			logger_.info("Check device request done {0}", response.ok());
			next();
			responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}
