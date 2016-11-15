#ifndef ICommandsImpl_Included
#define ICommandsImpl_Included

#include "executable_command_base.hpp"

namespace access_device
{
	namespace commands
	{		
		class LightCommand final: public ExecutableCommandBase
		{
		public:
			LightCommand(): ExecutableCommandBase(rs232::WRITE_COMMAND_SIZE)	{	
				input_command_ = std::make_unique<WritePortCommand>(rs232::port_command::WriteB);
			}
		};

		class ButtonCommand final : public ExecutableCommandBase
		{
		public:
			ButtonCommand() : ExecutableCommandBase(rs232::WRITE_COMMAND_SIZE)	{
				input_command_ = std::make_unique<ReadPortCommand>(rs232::port_command::ReadC);
			}
		};

		class DallasCommand final : public ExecutableCommandBase
		{
		public:
			DallasCommand() : ExecutableCommandBase(rs232::DALLAS_SIZE)	{
				input_command_ = std::make_unique<ReadPortCommand>(rs232::port_command::Dallas);
			}
		};

		class DeviceInfoCommand final : public ExecutableCommandBase
		{
		public:
			DeviceInfoCommand() : ExecutableCommandBase(rs232::WRITE_COMMAND_SIZE)	{
				input_command_ = std::make_unique<ReadPortCommand>(rs232::port_command::ReadB);
			}
		};	
	}
}

#endif
