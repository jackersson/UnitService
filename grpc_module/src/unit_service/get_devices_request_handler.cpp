#include "unit_service/get_devices_request_handler.hpp"

#include <helpers/request_adapters.hpp>
#include <data/models/devices.hpp>

namespace grpc_services
{
	namespace unit_service
	{
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
			logger_.info("Get device request -> in");

			if  ( !try_resolve_dependencies() )
			{
				complete(devices);
				return;
			}
	
			context_->devices()->enumerate(devices);
			complete(devices);
		}

		void GetDevicesRequestHandler::complete(const data_model::Devices& devices)
		{
			auto response = services_api::helpers::to_proto_devices(devices);
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
