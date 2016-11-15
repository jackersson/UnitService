#include <access_device/commands/command_factory.hpp>

namespace access_device
{
	namespace commands {
		
		CommandFactory::CommandFactory() 			                           
		{
			//register_command<LightCommandImpl> (std::make_shared<LightCommandImpl>());
			register_command<ButtonCommand    >(std::make_shared<ButtonCommand>());
			register_command<DallasCommand    >(std::make_shared<DallasCommand>());
			register_command<DeviceInfoCommand>(std::make_shared<DeviceInfoCommand>());
		}	
		
		bool CommandFactory::reset(TimeoutSerial& sp)
		{
			std::vector<std::shared_ptr<core::IExecutableCommand>> commands_to_reset =
			{
				  get<ButtonCommand>()
				, get<LightCommand >()
				, get<DallasCommand>()
			};

			auto success = 0;
			for (auto cmd : commands_to_reset)
			{
				try
				{
					const auto& result = cmd->execute(sp);
					if (result->is_valid())
						success++;
				}
				catch (std::exception&){
					//Not implemented
				}
				
			}

			return success == commands_to_reset.size();
		}	

		int CommandFactory::get_device_number(TimeoutSerial& sp)
		{
			auto command = get<DeviceInfoCommand>();
			try
			{
				const auto& result = command->execute(sp);
				return result->is_valid() ? result->device_number() : DEVICE_ERROR;
			}
			catch (std::exception&){
				return DEVICE_ERROR;
			}
		}
	}
}
