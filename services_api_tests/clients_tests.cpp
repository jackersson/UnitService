#include <gtest/gtest.h>
#include <coordinator_service/coordinator_client.hpp>

namespace directshow_tests
{
	TEST(DirectshowEnumeratorTest, DirectshowDeviceTests)
	{
		contracts::services::ServiceAddress addr("127.0.0.1:");
		grpc_services::CoordinatorClient client(addr);

		EXPECT_TRUE(true);
	}

}
