#include <access_device/commands/command_factory.hpp>
#include <cstdint>

namespace access_device
{
	namespace commands {
		
		CommandFactory::CommandFactory( uint16_t device_number) 
			                            : device_number_(device_number)
		{
			//register_command<LightCommandImpl> (std::make_shared<LightCommandImpl>());
			register_command<ButtonCommandImpl>(std::make_shared<ButtonCommandImpl>());
			register_command<DallasCommandImpl>(std::make_shared<DallasCommandImpl>());
		}

		std::shared_ptr<core::ICommandContext>
			CommandFactory::get(rs232::port_command id, unsigned char data)
		{
			auto it = commands_.find(id);
			std::shared_ptr<core::ICommandContext> command;
			if (it != commands_.end())
				command = it->second;
			else
				command = std::make_shared<LightCommandImpl>();
			apply_settings(*command, data);
			return command;
		}
		
		bool CommandFactory::reset(TimeoutSerial& sp)
		{
			std::vector<std::shared_ptr<core::ICommandContext>> commands_to_reset =
			{
				  get<ButtonCommandImpl>(0)
				, get<LightCommandImpl>(0)
				, get<DallasCommandImpl>(0)
			};

			auto success = 0;
			for (auto cmd : commands_to_reset)
			{
				const auto& result = cmd->execute(sp);
				if (result->ok())
					success++;
			}

			return success == commands_to_reset.size();
		}	
	}
}
