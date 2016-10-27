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
			//TODO maybe remove from here
			const Command& input() const override	{
				return *input_command_.get();
			}

		private:
			std::unique_ptr<Command> input_command_;
		};

		class ButtonCommand final : public ExecutableCommandBase
		{

		public:
			ButtonCommand()
				: ExecutableCommandBase(rs232::port_command::ReadC, rs232::DATA_SIZE)
			{
				std::vector<unsigned char> bytes;
				rs232::create_empty_command(bytes, id(), 0);
				input_command_ = std::make_unique<Command>(bytes);
			}

			//TODO maybe remove from here
			const Command& input() const override {
				return *input_command_.get();
			}

		protected:
			std::shared_ptr<Command> create_command(const std::vector<unsigned char>& bytes) const override
			{
				return std::make_shared<InvertedCommand>(bytes);
			}


		private:
			std::unique_ptr<Command> input_command_;
		};

		class DallasCommand final : public ExecutableCommandBase
		{
		public:
			DallasCommand()
				: ExecutableCommandBase(rs232::port_command::Dallas, rs232::DALLAS_SIZE)
			{
				std::vector<unsigned char> bytes;
				rs232::create_empty_command(bytes, id(), 0);
				input_command_ = std::make_unique<Command>(bytes);		
			}

			//TODO maybe remove from here
			const Command& input() const override {
				return *input_command_.get();
			}

		protected:
			std::shared_ptr<Command> 
				create_command(const std::vector<unsigned char>& bytes) const override
			{
				return std::make_shared<InvertedCommand>(bytes);
			}


		private:
			std::unique_ptr<Command> input_command_;
		};
	
	}
}

#endif
