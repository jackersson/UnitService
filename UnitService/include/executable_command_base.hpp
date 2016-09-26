#ifndef ExecutableCommandBase_Included
#define ExecutableCommandBase_Included
#include <cstdint>
#include <asio/serial_port.hpp>
#include "command.hpp"

#include "rs232_controller_types.hpp"
#include "serial_port_io.hpp"

namespace utils
{
	class ExecutableCommandBase
	{
	public:
		ExecutableCommandBase( rs323_controller::port_command id
			                   , uint32_t response_bytes_count) 
			                   : id_(id)
			                   , count_(response_bytes_count)
		{
			
		}

		void execute( boost::asio::serial_port& sp
			          , uint16_t device_number
			          , unsigned char data)
		{
			auto input_command = input();
			auto input_bytes   = input_command.bytes();
			rs323_controller::update_command(input_bytes, device_number, data);
			auto header = rs323_controller::get_header(input_bytes);

			if (!input_command.valid())
				return Result.From(ExecutionResult.NotValid);

			try
			{
				do_execute(sp, input_bytes, header);
			}
			catch (std::exception& ex)	{
				Result.Error(exception);
			}

			auto command = create_command(response_);

			if (empty(command))
				return Result.Data(command.Data);

			return !valid(command)
				? Result.From(ExecutionResult.NotValid)
				: Result.Data(command.Data);
		}

		void do_execute( boost::asio::serial_port& sp
			                        , const std::vector<unsigned char>& bytes
			                        , unsigned char header )
		{			
			auto result = serial_port_io_.execute(sp, bytes, response_, count_, header);

			if (!result)
				throw std::exception("Execution wasn't successfull");				
		}

	protected:
	  virtual Command create_command(const std::vector<unsigned char>& bytes)
		{
			return Command(bytes);
		}

		virtual bool valid(const Command& command)
		{
			return command.valid();
		}

		virtual bool empty(const Command& command)
		{
			return command.empty();
		}

		virtual const Command& input() const = 0;

		virtual ~ExecutableCommandBase() {}

	private:
		rs323_controller::port_command id_;
		uint32_t count_;
		access_device::SerialPortIO serial_port_io_;
		std::vector<unsigned char> response_;
	};
}

#endif
