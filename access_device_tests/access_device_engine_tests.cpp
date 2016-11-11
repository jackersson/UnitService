#include <gtest/gtest.h>
#include <access_device/access_device_engine.hpp>

namespace unit_service_api_tests
{
	TEST(AccessDeviceEngineTest, OpenDoorTest)
	{
		/*
		std::string device_name = "COM3";

		access_device::AccessDeviceEngine engine;
		engine.init();		
		std::this_thread::sleep_for(std::chrono::seconds(1));

		std::string result = "";
		try {
			result = engine.grant_access(device_name);
		}
		catch (std::exception& exception)	{
			EXPECT_EQ("Unknown exception", exception.what());
		}
		EXPECT_NE("", result);		

		engine.de_init();
		*/
	}

	//Put card to reader while running test
	TEST(AccessDeviceEngineTest, GetCardTest)
	{
		/*
		std::string device_name = "COM3";

		access_device::AccessDeviceEngine engine;
		engine.init();
		std::this_thread::sleep_for(std::chrono::seconds(1));

		std::string result = "";
		try {
			result = engine.get_card(device_name);
		}
		catch (std::exception& exception)
		{
			EXPECT_EQ("Unknown exception", exception.what());
		}
		EXPECT_NE("", result);

		engine.de_init(); */
	}

	//Connect any access device before passing test and check lights
	TEST(AccessDeviceEngineTest, CheckDeviceTest)
	{
		/*
		std::string device_name = "COM3";

		access_device::AccessDeviceEngine engine;
		engine.init();
		std::this_thread::sleep_for(std::chrono::seconds(1));

		auto result = false;
		try	{
			result = engine.check(device_name);
		}
		catch (std::exception& exception)	{
			EXPECT_EQ("Unknown exception", exception.what());
		}
		EXPECT_TRUE(result);

		engine.de_init(); */
	}


	TEST(AccessDeviceEngineTest, AccessDeviceLifecycleTest)
	{
		std::string device_name = "COM3";

		access_device::AccessDeviceEngine engine;
		engine.init();

		engine.add(device_name);

		std::chrono::milliseconds delay_(10000);
		std::this_thread::sleep_for(delay_);
		engine.de_init();
	}
}
