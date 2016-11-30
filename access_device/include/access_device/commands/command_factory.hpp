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
			core::IExecutableCommandPtr
				get(uint16_t device_number = 0, unsigned char data = 0)
			{						
				auto it = container_.find(typeid(T).name());
				core::IExecutableCommandPtr command;
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
			void register_command(core::IExecutableCommandPtr command)
			{
				if (container_.find(typeid(T).name()) != container_.end())
					throw std::exception("Access Device Command already registered in command factory");
			
				container_.insert(std::pair<std::string
					, core::IExecutableCommandPtr>(typeid(T).name(), command));
			}
		
			std::map<std::string, core::IExecutableCommandPtr> container_;

		};
	}
}

#endif