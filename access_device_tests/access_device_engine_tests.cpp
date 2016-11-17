#include <gtest/gtest.h>
#include <access_device/access_device_engine.hpp>

#include <data/models/devices.hpp>
#include "simple_threadable.hpp"
#include "test_command_context.hpp"

using namespace concurrent::containers;
using namespace access_device;
using namespace contracts::devices::access_device;

namespace unit_service_api_tests
{
	TEST(AccessDeviceEngineTest, OpenDoorTest)
	{
		data_model::DeviceId device_name("test", 0);

		AccessDeviceEngine engine;
		std::this_thread::sleep_for(std::chrono::seconds(1));

		engine.add    (device_name);
		engine.execute(device_name, lGreenAccess);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		engine.de_init();		
	}

	//Put card to reader while running test
	TEST(AccessDeviceEngineTest, GetCardTest)
	{
		data_model::DeviceId device("COM3", 0);
		access_device_tests::TestableObserver observer;
		AccessDeviceEngine engine;
		std::this_thread::sleep_for(std::chrono::seconds(1));

		engine.add(device);
		engine.subscribe(&observer, device);
		std::this_thread::sleep_for(std::chrono::seconds(10));
		engine.remove(device);

		engine.de_init(); 
	}

	//Connect any access device before passing test and check lights
	TEST(AccessDeviceEngineTest, CheckDeviceTest)
	{		
		data_model::DeviceId device("COM3", 0);

		AccessDeviceEngine engine;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		engine.add(device);
		engine.execute(device, lGreenAccess);
		engine.execute(device, lNone       );
		engine.execute(device, lRedMain    );
		engine.execute(device, lGreenAccess);
		engine.execute(device, lNone       );
		engine.execute(device, lRedMain    );
		std::this_thread::sleep_for(std::chrono::seconds(1));
		engine.de_init();
	}


	TEST(AccessDeviceEngineTest, AccessDeviceLifecycleTest)
	{
		data_model::DeviceId device_name("Com", 0);

		AccessDeviceEngine engine;

		engine.add(device_name);

		std::chrono::seconds delay_(20);
		std::this_thread::sleep_for(delay_);
		engine.de_init();
	}

	TEST(AccessDeviceEngineTest, EnumeratorTest)
	{
		SerialPortEnumerator enumerator;	
		enumerator.start();	
		std::this_thread::sleep_for(std::chrono::milliseconds(20000));

		enumerator.stop();
	}

	TEST(AccessDeviceEngineTest, EnumeratorUpdateTest)
	{
		AccessDeviceEngine engine;

		std::this_thread::sleep_for(std::chrono::milliseconds(30000));

		engine.de_init();
	}

	
	TEST(AccessDeviceEngineTest, ConcurrentMapTest)
	{
		ConcurrentMap<std::string, AccessDeviceListenerPtr> mp;
		auto count = 5;

		for ( auto i = 0; i < count; ++i)
	  	mp.insert
			(   std::to_string(i)
				, std::make_shared<AccessDeviceListener>(data_model::DeviceId(i)
				, nullptr));
		EXPECT_EQ(count, mp.size());

		for (auto i = 0; i < count; ++i)
			mp.remove(std::to_string(i));
		EXPECT_EQ(0, mp.size());
	}

	TEST(AccessDeviceEngineTest, ConcurrentVectorTest)
	{
		ConcurrentVector<AccessDeviceListenerPtr> mp;
		auto count = 5;

		for (auto i = 0; i < count; ++i)
			mp.push_back
			(std::make_shared<AccessDeviceListener>(data_model::DeviceId(i)
				                                     , nullptr));
		EXPECT_EQ(count, mp.size());

		mp.clear();
		EXPECT_EQ(0, mp.size());
	}

	TEST(AccessDeviceEngineTest, SimpleThreadTest)
	{		
		utils::SimpleThreadable thread;

		thread.start();
		std::this_thread::sleep_for(std::chrono::nanoseconds(100));
		thread.stop();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		thread.start();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		thread.stop();
	}
}
