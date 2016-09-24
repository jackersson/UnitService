#ifndef IExecutableCommand_Included
#define IExecutableCommand_Included
#include <asio/serial_port.hpp>
#include "rs232_controller_types.hpp"
#include "access_device_types.hpp"


namespace access_device
{
	class ICommandResult
	{
	public:
		virtual ~ICommandResult()	{}

		virtual access_device_module module() = 0;
		virtual std::vector<unsigned char>& data() = 0;

		virtual std::string    to_string() = 0;
		virtual std::exception exception() = 0;		

		virtual bool is_ok()    = 0;
		virtual bool is_empty() = 0;
	};

	class IExecutableCommand
	{
	public:
		virtual ~IExecutableCommand()	{}

		virtual const ICommandResult& execute(boost::asio::serial_port& sp) = 0;

		virtual bool reset(boost::asio::serial_port& sp) = 0;

		virtual port_command id() = 0;
	};

	


}

#endif