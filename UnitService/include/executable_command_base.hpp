#ifndef ExecutableCommandBase_Included
#define ExecutableCommandBase_Included
#include <cstdint>
#include <asio/serial_port.hpp>
#include "command.hpp"

#include "rs232_controller_types.hpp"

namespace utils
{
	class ExecutableCommandBase
	{
	public:
		ExecutableCommandBase( rs323_controller::port_command id
			                   , uint32_t response_bytes_count) 
			: id_(id)
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

			auto exception = TryExecute(ref serialPort, bytes, header);

			if (exception != null)
				return Result.Error(exception);

			var command = CreateCommand(_response);

			if (IsEmpty(command))
				return Result.Data(command.Data);

			return !IsValid(command)
				? Result.From(ExecutionResult.NotValid)
				: Result.Data(command.Data);
		}

		std::exception try_execute( boost::asio::serial_port& sp
			                        , const std::vector<unsigned char>& bytes
			                        , unsigned char header )
		{
			try
			{
				var result = _operator.Execute(ref serialPort
					, bytes
					, ref _response
					, header);

				if (!result)
					throw new InvalidOperationException("Execution wasn't successfull");
				return null;
			}
			catch (Exception exception)
			{
				return exception;
			}
		}

		virtual const Command& input() const = 0;

		virtual ~ExecutableCommandBase() {}

	private:
		rs323_controller::port_command id_;
	};
}

#endif
