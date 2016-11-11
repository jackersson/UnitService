#include <gtest/gtest.h>
#include <devices_container.hpp>
#include <data/models/devices.hpp>

namespace unit_service_api_tests
{		
	//Before test connect devices of all types
	TEST(UnitServiceApiTests, GetDevicesTest)
	{
		DevicesContainer devices;
		devices.init();

		std::this_thread::sleep_for(std::chrono::seconds(1));

		data_model::Devices devs;
		devices.enumerate(devs);
		devices.de_init();

		EXPECT_TRUE(devs.size() > 0);

		auto access_device_found  = false;
		auto capture_device_found = false;
		for (auto dev : devs)
		{
			switch( dev.type() )
			{
			case data_model::DeviceType::None_Type: break;
			case data_model::DeviceType::CardReader:
				access_device_found = true;
				break;
			case data_model::DeviceType::Capture:
				capture_device_found = true;
				break;			
			default: break;
			}
		}

		EXPECT_TRUE(access_device_found );
		EXPECT_TRUE(capture_device_found);
	}

}
