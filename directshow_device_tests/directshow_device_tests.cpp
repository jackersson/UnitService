#include <gtest/gtest.h>
#include <directshow_device_enumerator.hpp>
#include <directshow_device_engine.hpp>
#include "directshow_device_observer.hpp"

#include <data/models/devices.hpp>

namespace directshow_tests
{
	TEST(DirectshowEnumeratorTest, DirectshowDeviceTests)
	{
	  directshow_device::DirectshowDeviceEnumerator enumerator;
	  enumerator.start();
	  
	  std::this_thread::sleep_for(std::chrono::seconds(3));
	  
	  enumerator.stop();
	  
	  EXPECT_TRUE(true);		
	}
		
	TEST(DirectshowEnumeratorTest, LifecycleTests)
	{
	  directshow_device::DirectShowDeviceEngine enumerator;
	  DirectshowDeviceObserver obse;
		data_model::DeviceId camera("LightCam 1020");
	  
	  enumerator.add(camera);
	  enumerator.subscribe(&obse, camera);

		std::this_thread::sleep_for(std::chrono::seconds(10));

		enumerator.unsubscribe(&obse);
		enumerator.remove(camera);
	  enumerator.de_init();
	}
	
}
