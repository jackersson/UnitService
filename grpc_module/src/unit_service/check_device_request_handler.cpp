/*#include "unit_service/check_device_request_handler.hpp"

#include <datatypes/devices.pb.h>
#include <data/models/devices.hpp>
#include <boost/thread/future.hpp>

using namespace devices;

namespace grpc_services
{
	namespace unit_service
	{

		CheckDeviceRequestHandler::CheckDeviceRequestHandler
		  ( Services::UnitService::AsyncService* service
			, grpc::ServerCompletionQueue* completion_queue
			, contracts::IServiceContext* context)
			: RequestHandler<Services::UnitService::AsyncService>(service, completion_queue)
			, initialized_(false)
			, responder_  (&server_context_)
			, context_    (context)
		{
			
			proceed();
		}			

		CheckDeviceRequestHandler::~CheckDeviceRequestHandler()
		{
			
		}

		void CheckDeviceRequestHandler::process_request()
		{
			logger_.info("Check device request -> in");
		
			if (!try_resolve_dependencies())
			{
				complete(DataTypes::CheckMsg());
				return;
			}

			if (request_.device_type() == DataTypes::DeviceType::CardReader)
			{
				auto serial = static_cast<uint16_t>(request_.serial_number());
				data_model::DeviceId dev("any", serial);

				boost::async([this, dev]() {
					return check_device(engine_->enumerator(), dev);
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

		bool CheckDeviceRequestHandler::try_resolve_dependencies()
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

				engine_ = devices->access_device_engine();
				if (engine_ == nullptr)
				  throw std::exception("Access Device engine can't be null");
				
				return true;
			}
			catch (std::exception& ex)
			{
				logger_.error("CheckDeviceRequestHandler::try_resolve_dependencies {0}", ex.what());
				return false;
			}
		}
	}
}
*/