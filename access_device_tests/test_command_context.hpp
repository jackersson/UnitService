#ifndef TestCommandContext_Included
#define TestCommandContext_Included

#include <access_device/access_device_listener.hpp>
#include <access_device/core/timeout_serial.hpp>
#include <gtest/gtest.h>

using namespace access_device;
using namespace commands     ;
using namespace core         ;

namespace access_device_tests
{
	class TestCommandContext
	{
	public:
		explicit TestCommandContext(const std::string& device_name)
			: serial_port_()
		{
			serial_port_.open(device_name, AccessDeviceListener::BAUD_RATE);
			serial_port_.set_timeout(boost::posix_time::millisec(1000));
		}

		~TestCommandContext() {
			serial_port_.close();
		}

		TimeoutSerial& serial_port(){
			return serial_port_;
		}

		ICommandResultPtr	execute(IExecutableCommandPtr command)	{
			return command->execute(serial_port_);
		}
	private:
		TimeoutSerial serial_port_;
	};

	class TestableObserver : public devices::IDeviceObserver<ICommandResultPtr>
	{
	public:
		void on_error(const devices::DeviceException& exception) override
		{		
			//EXPECT_EQ("Undefined exception", exception.what());
		}

		void on_state(const devices::IDeviceState& state) override
		{		
		}

		void on_next(ICommandResultPtr data) override
		{
			switch (data->device_module())
			{
			case NoneModule:
				EXPECT_TRUE(false);
				break;
			case Info   : 
				EXPECT_TRUE(false);
				break;
			case Lights : 
				EXPECT_TRUE(false);
				break;
			case Buttons: 
				std::cout << "button " << data->small_data() << std::endl;
				break;
			case Dallas: 
			{			
				auto dallas_key = data->get_dallas_key();
			  std::cout << "dallas key" << dallas_key << std::endl;
			  EXPECT_NE("", dallas_key);
			  break;
			}
			default: break;
			}			
		}
	};
}

#endif
