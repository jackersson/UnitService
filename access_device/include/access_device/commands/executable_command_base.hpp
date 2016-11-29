#ifndef ExecutableCommandBase_Included
#define ExecutableCommandBase_Included

#include <cstdint>
#include <devices/access_device/icommand_result.hpp>
#include "access_device/core/serial_port_io.hpp"
#include "command.hpp"
#include <access_device/core/iexecutable_command.hpp>

namespace access_device
{
	namespace commands
	{
		class Command;
		class ExecutableCommandBase : public core::IExecutableCommand
		{
		public:
			explicit ExecutableCommandBase(uint32_t response_bytes_count);

			virtual ~ExecutableCommandBase();

			ICommandResultPtr	 execute(TimeoutSerial& sp ) override;

			bool reset(TimeoutSerial& sp) override;
			
			IInputCommand& command() const override {
				return *input_command_;
			}

		protected:
			std::unique_ptr<IInputCommand> input_command_;

		private:
			void do_execute( TimeoutSerial& sp
				             , const std::vector<unsigned char>& bytes
				             , char header);

			uint32_t                   count_         ; 
			SerialPortIO               serial_port_io_;
			std::vector<unsigned char> response_      ;
		};
	}
}

#endif
