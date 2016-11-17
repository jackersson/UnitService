#include <gtest/gtest.h>
#include <data/models/devices.hpp>
#include <access_device/access_device_engine.hpp>
#include "testable_check_device_request.hpp"
#include "testable_get_card_request.hpp"
#include <devices_container.hpp>

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
		for (const auto& dev : devs)
		{
			switch (dev.type())
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

	//TODO check on reserved engine
	//Plug unplag wanted device
	TEST(UnitServiceApiTests, CheckDeviceTest)
	{
		access_device::AccessDeviceEngine engine;
		testable_unit_context::CheckDeviceMock check(&engine);

		DataTypes::Device dev;
		dev.set_serial_number(0);
		dev.set_device_type(DataTypes::CardReader);
		check.process_request(dev);
	}

	TEST(UnitServiceApiTests, GetCardTest)
	{
		access_device::AccessDeviceEngine engine;
		testable_unit_context::GetCardMock check(&engine);

		DataTypes::Device dev;
		dev.set_serial_number(0);
		dev.set_device_type(DataTypes::CardReader);
		check.process_request(dev);
	}

	TEST(UnitServiceApiTests, OpenDoorTest)
	{
		//TrackLocationEngine e
		access_device::AccessDeviceEngine engine;
		//acce engine;
		//testable_unit_context::GetCardMock check(&engine);

		//DataTypes::Device dev;
		//dev.set_serial_number(0);
		//dev.set_device_type(DataTypes::CardReader);
		//check.process_request(dev);
	}

}
