#ifndef CommandFactory_Included
#define CommandFactory_Included

#include <map>
#include <access_device/core/iexecutable_command.hpp>
#include "command_impl.hpp"

namespace access_device
{
	namespace commands {			
		
		class CommandFactory 
		{
		public:
			CommandFactory();			

			template <typename T>
			std::shared_ptr<core::IExecutableCommand>
				get(uint16_t device_number = 0, unsigned char data = 0)
			{				
				//TODO should be smarter way.
				//The problem: when data applied to one object it changes everywhere
				//Possible solution: try to use pool of objects instead of single object
				auto it = container_.find(typeid(T).name());
				std::shared_ptr<core::IExecutableCommand> command;
				if (it != container_.end())				
					command = it->second;							
				else				
					command = std::make_shared<LightCommand>();	
				command->command().set_data         (data         );
				command->command().set_device_number(device_number);
				return command;
			}			

			bool reset(TimeoutSerial& sp);			

			int get_device_number(TimeoutSerial& sp);
			static const int DEVICE_ERROR = -1;

		private:
			template<typename T>
			void register_command(std::shared_ptr<core::IExecutableCommand> command)
			{
				//TODO check if exists
				container_.insert(std::pair<std::string
					, std::shared_ptr<core::IExecutableCommand>>(typeid(T).name(), command));
			}
		
			std::map<std::string
				, std::shared_ptr<core::IExecutableCommand>> container_;

		};
	}
}

#endif