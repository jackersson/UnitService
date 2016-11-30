#include "unit_service/get_devices_request_handler.hpp"

#include <helpers/request_adapters.hpp>
#include <data/models/devices.hpp>

using namespace services_api::helpers;

namespace grpc_services
{
	namespace unit_service
	{
		std::string GetDevicesRequestHandler::devices_[3] = {
				"all devices"
			, "access devices"
			, "capture devices"
		};

		GetDevicesRequestHandler::GetDevicesRequestHandler(Services::UnitService::AsyncService* service
			, grpc::ServerCompletionQueue*    completion_queue
			, contracts::IServiceContext*       context)
			: RequestHandler<Services::UnitService::AsyncService>(service, completion_queue)
			, responder_(&server_context_)
			, context_(context)
			, initialized_(false)
		{
			proceed();
		}
		void GetDevicesRequestHandler::process_request()
		{
			data_model::Devices devices;

			

			if  ( !try_resolve_dependencies() )
			{
				complete(devices);
				return;
			}

			auto type = to_data_device_type(request_.dev_type());			
			logger_.info("Get {0} request -> in", devices_[type]);

			context_->devices()->enumerate(devices, type);
			complete(devices);
		}

		void GetDevicesRequestHandler::complete(const data_model::Devices& devices)
		{
			auto response = to_proto_devices(devices);
			logger_.info("Get device request -> out {0}", devices.size());

			responder_.Finish(response, grpc::Status::OK, this);
			next();
		}

		bool GetDevicesRequestHandler::try_resolve_dependencies()
		{
			if (initialized_)
				return initialized_;

			try
			{
				initialized_ = true;

				if (context_ == nullptr)
					throw std::exception("Context can't be null");

				auto devices = context_->devices();
				if (devices == nullptr)
					throw std::exception("Devices can't be null");

				return true;
			}
			catch (std::exception& ex)
			{
				logger_.error("GetDeviceStreamRequestHandler::try_resolve_dependencies {0}", ex.what());
				return false;
			}
		}
	}
}
