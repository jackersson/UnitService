#ifndef IExecutableCommand_Included
#define IExecutableCommand_Included

#include <asio/serial_port.hpp>

#include <contracts/devices/access_device/icommand_result.hpp>
#include <access_device/rs232/rs232_controller_types.hpp>

namespace access_device
{
	namespace core {
		typedef contracts::devices::access_device::ICommandResult ICommandResult;
		class IExecutableCommand
		{
		public:

			virtual ~IExecutableCommand() {}
			virtual const ICommandResult& execute(boost::asio::serial_port& sp) = 0;

			virtual bool reset(boost::asio::serial_port& sp) = 0;

			virtual rs232::port_command id() const = 0;
		};

		typedef std::shared_ptr<IExecutableCommand> IExecutableCommandPtr;

		class ICommandSettings
		{
		public:
			virtual ~ICommandSettings() {}

			virtual void set_device_number(uint16_t dev_number) = 0;
			virtual void set_data         (uint32_t data      ) = 0;

			virtual uint16_t device_number() const = 0;
			virtual uint32_t data         () const = 0;
		};

		class ICommandContext: public IExecutableCommand, public ICommandSettings
		{
		public:
			virtual ~ICommandContext() {}
		};
	}
}

#endif