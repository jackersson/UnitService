#ifndef CommandFactory_Included
#define CommandFactory_Included

#include <access_device/rs232/rs232_controller_utils.hpp>
#include <vector>
#include <map>
#include <access_device/core/iexecutable_command.hpp>

namespace access_device
{
	namespace commands {			
		
		class CommandFactory 
		{
		public:
			explicit CommandFactory(uint16_t device_number)	: device_number_(device_number)
			{
				//TODO initialization
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

			bool reset(boost::asio::serial_port& sp)
			{
				auto success = 0;
				for ( auto it : commands_)
				{
					auto cmd = it.second;
					apply_settings(*cmd.get());
					const auto& result = cmd->execute(sp);

					if (result.ok())
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
			uint16_t device_number_;
			std::map< rs232::port_command
				      , std::shared_ptr<core::ICommandContext>> commands_;
		};
	}
}

#endif