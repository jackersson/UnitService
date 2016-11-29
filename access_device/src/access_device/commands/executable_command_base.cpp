#include <cstdint>

#include <access_device/rs232/rs232_controller_types.hpp>
#include <access_device/core/serial_port_io.hpp>
#include <access_device/commands/executable_command_base.hpp>
#include <access_device/commands/command.hpp>

namespace access_device
{
	using namespace rs232;

	namespace commands
	{	
		ExecutableCommandBase::ExecutableCommandBase(uint32_t response_bytes_count)
			: count_(response_bytes_count)				
		{}

		ExecutableCommandBase::~ExecutableCommandBase() {}

		ICommandResultPtr	ExecutableCommandBase::execute( TimeoutSerial& sp )
		{					
		  do_execute(sp, input_command_->get(), input_command_->get_header());
			
			auto output = std::make_shared<OutputCommand>(response_);
			if (!output->is_valid())
				throw std::exception("output command isn't valid");

			return output;		
		}

		void ExecutableCommandBase::do_execute(TimeoutSerial& sp
			                          , const std::vector<unsigned char>& bytes
			                          , char header)
		{
			auto result = serial_port_io_.execute(sp, bytes, response_, count_, header);

			if (!result)
				throw std::exception("Execution wasn't successfull");
		}

		bool ExecutableCommandBase::reset( TimeoutSerial& sp )
		{
			auto response = execute(sp);
			return response->is_valid();
		}	
	}
}

