#ifndef TestCommandContext_Included
#define TestCommandContext_Included

#include <asio/serial_port.hpp>
#include <access_device/access_device_listener.hpp>
#include <gtest/gtest.h>

namespace access_device_tests
{
	class TestCommandContext
	{
	public:
		explicit TestCommandContext(const std::string& device_name)
			: serial_port_()
		{
			serial_port_.open(device_name, access_device::AccessDeviceListener::BAUD_RATE);
			serial_port_.set_timeout(boost::posix_time::millisec(100));
		}

		~TestCommandContext() {
			serial_port_.close();
		}

		contracts::devices::access_device::ICommandResultPtr
			execute(access_device::core::ICommandContextPtr command)
		{
			return command->execute(serial_port_);
		}

		static void
			check_result(contracts::devices::access_device::ICommandResultPtr result)
		{
			EXPECT_TRUE(std::string(result->exception().what()) == "Unknown exception");
			EXPECT_TRUE(result->ok());
			EXPECT_FALSE(result->empty());
			EXPECT_TRUE(result->data().size() > 0);
		}

	private:
		access_device::TimeoutSerial serial_port_;
	};
}

#endif
