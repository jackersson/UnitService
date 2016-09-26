#ifndef IExecutableCommand_Included
#define IExecutableCommand_Included
#include <asio/serial_port.hpp>
#include "rs232_controller_types.hpp"
#include "access_device_types.hpp"
#include "icommand_result.hpp"

namespace access_device
{
	
	class IExecutableCommand
	{
	public:
		virtual ~IExecutableCommand()	{}
		virtual const ICommandResult& execute(boost::asio::serial_port& sp) = 0;

		virtual bool reset(boost::asio::serial_port& sp) = 0;

		virtual rs323_controller::port_command id() = 0;
	};

	typedef std::shared_ptr<IExecutableCommand> IExecutableCommandPtr;


}

#endif