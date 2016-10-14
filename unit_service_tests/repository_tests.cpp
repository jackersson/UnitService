#include <gtest/gtest.h>
#include <launcher.hpp>


namespace repository_tests
{
	
	TEST(GetLocationTests, GetLocationTests)
	{
		Launcher launcher;
		launcher.init();

		auto context = launcher.context();
		EXPECT_NE(nullptr, context);

		std::vector<DataTypes::Location> items;
		auto request = new DataTypes::GetLocationRequest();
		context->repository()->locations()->get(request, items);
		EXPECT_TRUE(items.size() > 0);
	}



}
