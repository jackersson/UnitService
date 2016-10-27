#include <gtest/gtest.h>
#include <launcher.hpp>
#include <devices_container.hpp>

namespace unit_service_api_tests
{		
	//Before test connect devices of all types
	TEST(UnitServiceApiTests, GetDevicesTest)
	{
		DevicesContainer devices;
		devices.init();

		std::this_thread::sleep_for(std::chrono::seconds(1));

		DataTypes::Devices devs;
		devices.enumerate(devs);
		devices.de_init();

		auto items = devs.devices();

		EXPECT_TRUE(items.size() > 0);

		auto access_device_found  = false;
		auto capture_device_found = false;
		for (auto dev : items)
		{
			switch( dev.device_type() )
			{
			case DataTypes::None_Type: break;
			case DataTypes::CardReader: 
				access_device_found = true;
				break;
			case DataTypes::Capture:
				capture_device_found = true;
				break;
			case DataTypes::DeviceType_INT_MIN_SENTINEL_DO_NOT_USE_: break;
			case DataTypes::DeviceType_INT_MAX_SENTINEL_DO_NOT_USE_: break;
			default: break;
			}
		}

		EXPECT_TRUE(access_device_found );
		EXPECT_TRUE(capture_device_found);
	}

}
