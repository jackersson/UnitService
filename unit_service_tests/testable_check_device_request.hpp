#ifndef TestableUnitContext_Included
#define TestableUnitContext_Included
#include <datatypes/devices.pb.h>
#include <access_device/access_device_engine.hpp>

#include <data/models/devices.hpp>
#include <gtest/gtest.h>

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#include <boost/thread/future.hpp>
#include <future>
#include <service_utils.hpp>
#include "unit_service_api.hpp"

namespace testable_unit_context
{

	class CheckDeviceMock
	{
	public:
		explicit CheckDeviceMock(access_device::AccessDeviceEngine* engine)
			: engine_(engine)
		{}

		//Could be async as well
		void process_request(const DataTypes::Device& request)
		{
			request_ = request;
			DataTypes::CheckMsg response;

			if (request_.device_type() == DataTypes::DeviceType::CardReader)
			{
				auto serial = static_cast<uint16_t>(request_.serial_number());
				data_model::DeviceId dev("any", serial);
					
				boost::async([this, dev]()	{
					return check_device(engine_->enumerator(), dev);
				})
				.then([this](boost::future<bool> f) {
						prepare_response(f.get());
				});
			}
			else
			{
				response.set_message("Device type not valid. Should be Access Device");
				complete(response);
			}
		}

		void complete(const DataTypes::CheckMsg& response)
		{			
			EXPECT_EQ  ("", response.message());
			EXPECT_TRUE(response.ok());
		}	

		void prepare_response(bool ok)
		{			
			DataTypes::CheckMsg response;
			response.set_ok(ok);
			if (!ok)
				response.set_message("Failed to connect to device");
			complete(response);			
		}
				
	private:
		DataTypes::Device request_;
		access_device::AccessDeviceEngine* engine_;

		const int REQUEST_TIMEOUT = 1;
	};
}

#endif

