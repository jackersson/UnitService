#ifndef CommandFactory_Included
#define CommandFactory_Included

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
			explicit CommandFactory(uint16_t device_number);			

			std::shared_ptr<core::ICommandContext>
				get(rs232::port_command id, unsigned char data = 0);			

			template <typename T>
			std::shared_ptr<core::ICommandContext>
				get(unsigned char data = 0)
			{				
				//TODO should be smarter way.
				//The problem: when data applied to one object it changes everywhere
				//Possible solution: try to use pool of objects instead of single object
				auto it = container_.find(typeid(T).name());
				std::shared_ptr<core::ICommandContext> command = nullptr;
				if (it != container_.end())				
					command = it->second;							
				else				
					command = std::make_shared<LightCommandImpl>();				
				apply_settings(*command, data);
				return command;
			}

			bool reset(TimeoutSerial& sp);			

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