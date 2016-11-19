#include <gtest/gtest.h>
#include <launcher.hpp>

#include <services/service_address.hpp>
#include <coordinator_service/coordinator_client.hpp>
#include <coordinator_connector.hpp>
#include <unit_service/unit_service_impl.hpp>
#include <server_manager.hpp>
#include "testable_unit_context.hpp"


namespace unit_service_tests
{
	TEST(LaucherTest, LauncherTest)
	{
		Launcher launcher;
		launcher.init();

		EXPECT_FALSE(launcher.configuration().empty());

		launcher.de_init();
	}

	//if file not exists it should display path to file and ask to setup it
	TEST(ConfigurationTest, LoadingConfigurationTest)
	{
		ServiceConfiguration configuration;
		try_load_config(configuration);

		EXPECT_FALSE(configuration.empty());
	}

	TEST(CoordinatorTest, WorkerTest)
	{
		ServiceConfiguration configuration;
		try_load_config(configuration);

		//CoordinatorServiceWorker worker(&configuration);

		contracts::services::ServiceAddress
			address(configuration.coordinator_service_address());
		grpc_services::CoordinatorClient client(&configuration, address);
		client.start();

		CoordinatorConnector connector(&client, &configuration);
		std::this_thread::sleep_for(std::chrono::seconds(30));

		client.stop();
	}

	TEST(CoordinatorTest, CoordinatorMsgConvertTest)
	{
		//ServiceConfiguration configuration;
		//try_load_config(configuration);

		//data_model::ConnectMsg msg;
		//msg.id = data_model::configuration.service_uuid();
	}

	TEST(UnitServiceTest, LifecycleTest)
	{
		Launcher launcher;
		launcher.init();

		//EXPECT_FALSE(launcher.configuration().empty());
		std::this_thread::sleep_for(std::chrono::seconds(60));
		launcher.de_init();
	}

	//Tests for some apis
	TEST(UnitServiceTest, ServerTest)
	{
		ServiceConfiguration configuration;
		try_load_config(configuration);
		testable_unit_context::TestableServiceContext context(&configuration);
		context.init();

		grpc_services::ServerManager server(&context);

		server.start();
		std::this_thread::sleep_for(std::chrono::seconds(60));
		server.stop();
		server.de_init();
	}
}
