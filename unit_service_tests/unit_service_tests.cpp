#include <gtest/gtest.h>
#include <launcher.hpp>

namespace unit_service_tests
{
	TEST(LoadingConfiguraionTest, LoadingConfiguraionTest)
	{
		Launcher launcher;
		launcher.init();

		EXPECT_FALSE(launcher.configuration().empty());

		launcher.de_init();
	}

}
