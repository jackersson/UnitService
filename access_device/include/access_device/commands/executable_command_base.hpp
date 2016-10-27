#ifndef ExecutableCommandBase_Included
#define ExecutableCommandBase_Included

#include <cstdint>
#include "command.hpp"

#include <access_device/rs232/rs232_controller_types.hpp>
#include <access_device/core/serial_port_io.hpp>
#include "port_command_result.hpp"

namespace access_device
{
	namespace commands
	{
		class ExecutableCommandBase
		{
		public:
			ExecutableCommandBase(rs232::port_command id
				, uint32_t response_bytes_count)
				: id_(id)
				, count_(response_bytes_count)
				, result_(id)
			{}

			contracts::devices::access_device::ICommandResultPtr 
			execute(TimeoutSerial& sp, uint16_t device_number, unsigned char data)
			{
				auto input_command = input();
				auto input_bytes = input_command.bytes();
				rs232::update_command(input_bytes, device_number, data);
				auto header = rs232::get_header(input_bytes);

				if (!input_command.valid())
					return result_.not_valid();

				try
				{
					do_execute(sp, input_bytes, header);
				}
				catch (std::exception& ex) {
					return result_.exception(ex);
				}

				auto command = create_command(response_);

				if (empty(*command.get()))
					return result_.empty();

				return !valid(*command.get())
					? result_.not_valid()
					: result_.data(command->data());
			}

			void do_execute(TimeoutSerial& sp
				, const std::vector<unsigned char>& bytes
				, char header)
			{
				auto result = serial_port_io_.execute(sp, bytes, response_, count_, header);

				if (!result)
					throw std::exception("Execution wasn't successfull");
			}

			bool reset(TimeoutSerial& sp, uint16_t deviceNumber)
			{
				auto response = execute(sp, deviceNumber, 0);
				return response->ok();
			}

			rs232::port_command id() const {
				return id_;
			}
		protected:
			virtual std::shared_ptr<Command>
				create_command(const std::vector<unsigned char>& bytes) const
			{
				return std::make_shared<Command>(bytes);
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
			rs232::port_command id_;
			uint32_t count_;
			SerialPortIO serial_port_io_;
			std::vector<unsigned char> response_;
			PortCommandResult result_;
		};
	}
}

#endif
