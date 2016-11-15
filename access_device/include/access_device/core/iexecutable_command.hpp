#ifndef IExecutableCommand_Included
#define IExecutableCommand_Included

#include <contracts/devices/access_device/icommand_result.hpp>

namespace access_device{
	namespace commands
	{
		class OutputCommand;
		class IInputCommand;
	}
	class TimeoutSerial;
}

namespace access_device
{
	namespace core {
		
		typedef contracts::devices::access_device::ICommandResult ICommandResult;
		class IExecutableCommand
		{
		public:

			virtual ~IExecutableCommand() {}
			virtual contracts::devices::access_device::ICommandResultPtr
				       execute(TimeoutSerial& sp) = 0;

			virtual bool reset(TimeoutSerial& sp) = 0;

			virtual commands::IInputCommand& command() const = 0;
		};

		typedef std::shared_ptr<IExecutableCommand> IExecutableCommandPtr;	
	}
}

#endif