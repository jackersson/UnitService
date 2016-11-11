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

	//if file not exists it should display path to file and ask to setup it
	TEST(ConfiguraionTest, LoadingConfiguraionTest)
	{
		ServiceConfiguration configuration;
		try_load_config(configuration);

		EXPECT_FALSE(configuration.empty());
	}

}
