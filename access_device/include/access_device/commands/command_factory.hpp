#ifndef CommandFactory_Included
#define CommandFactory_Included

#include <access_device/rs232/rs232_controller_utils.hpp>
#include <vector>
#include <map>
#include <access_device/core/iexecutable_command.hpp>
#include "rs232_command.hpp"

namespace access_device
{
	namespace commands {			
		
		class CommandFactory 
		{
		public:
			explicit CommandFactory(uint16_t device_number)	: device_number_(device_number)
			{
				register_command<LightCommandImpl> (std::make_shared<LightCommandImpl>());
				register_command<ButtonCommandImpl>(std::make_shared<ButtonCommandImpl>());
				register_command<DallasCommandImpl>(std::make_shared<DallasCommandImpl>());
			}

			std::shared_ptr<core::ICommandContext>
			get(rs232::port_command id, unsigned char data = 0)
			{
				auto it = commands_.find(id);
				if (it != commands_.end())
				{
					apply_settings(*(it->second.get()), data);
					return it->second;
				}

				return nullptr;
			}

			template <typename T>
			std::shared_ptr<core::ICommandContext>
				get(unsigned char data = 0)
			{
				auto it = container_.find(typeid(T).name());
				if (it != container_.end())
				{
					apply_settings(*(it->second.get()), data);
					return it->second;
				}

				return nullptr;
			}

			bool reset(TimeoutSerial& sp)
			{
				auto success = 0;
				for ( auto it : commands_)
				{
					auto cmd = it.second;
					apply_settings(*cmd.get());
					const auto& result = cmd->execute(sp);

					if (result->ok())
						success++;
				}

				return success = commands_.size();
			}

			void apply_settings( core::ICommandSettings& command
				                 , unsigned char data = 0 ) const 
			{
				command.set_data(data);
				command.set_device_number(device_number_);
			}

		private:
			template<typename T>
			void register_command(std::shared_ptr<core::ICommandContext> command)
			{
				//TODO check if exists
				container_.insert(std::pair<std::string
					, std::shared_ptr<core::ICommandContext>>(typeid(T).name(), command));

				commands_.insert(std::pair<rs232::port_command
					, std::shared_ptr<core::ICommandContext>>(command->id(), command));
			}

			uint16_t device_number_;
			std::map< rs232::port_command
				      , std::shared_ptr<core::ICommandContext>> commands_;

			std::map<std::string
				, std::shared_ptr<core::ICommandContext>> container_;
		};
	}
}

#endif