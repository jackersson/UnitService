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
			LightCommand()
				: ExecutableCommandBase(rs232::port_command::WriteB, rs232::DATA_SIZE)
			{
				std::vector<unsigned char> bytes;
				rs232::create_data_command(bytes, id(), 0, 0);
				input_command_ = std::make_unique<Command>(bytes);
			}
	
			const Command& input() const override	{
				return *input_command_.get();
			}

		private:
			std::unique_ptr<Command> input_command_;
		};
	
	}
}

#endif
