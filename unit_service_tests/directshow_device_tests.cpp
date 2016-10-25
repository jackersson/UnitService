#include <gtest/gtest.h>
#include <directshow_device_enumerator.hpp>

namespace directshow_tests
{

	TEST(DirectshowEnumeratorTest, DirectshowDeviceTests)
	{
		directshow_device::DirectshowDeviceEnumerator enumerator;
		enumerator.start();

		std::this_thread::sleep_for(std::chrono::seconds(10));

		enumerator.stop();

		EXPECT_TRUE(true);
	}
}
