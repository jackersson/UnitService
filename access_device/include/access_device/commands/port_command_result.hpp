#ifndef CommandResultPool_Included
#define CommandResultPool_Included

#include <vector>
#include <contracts/devices/access_device/access_device_types.hpp>
#include <contracts/devices/access_device/icommand_result.hpp>
#include "command.hpp"
#include "command_result.hpp"

namespace access_device
{
	namespace commands {
		class PortCommandResult
		{
			typedef contracts::devices::access_device::access_device_module dev_module;

		public:
			PortCommandResult(rs232::port_command command	)
				: module_(to_device_module(command))
			{
				result_.set_module(module_);
			}

			//to utils
			dev_module to_device_module(rs232::port_command command)
			{
				switch (command)
				{
				case rs232::WriteB: return dev_module::Lights ;
				case rs232::ReadC : return dev_module::Buttons;
				case rs232::Dallas: return dev_module::Dallas ;
				case rs232::ReadB :
				case rs232::Master:
				default:
					break;
				}
				return dev_module::NoneModule;
			}

			virtual ~PortCommandResult() {}

			CommandResult not_valid()
			{
				clear();
				std::exception ex("Command check sum not valid");
				result_.set_exception(ex);
				return result_;
			}

			CommandResult exception(const std::exception& excetpion)
			{
				clear();
				result_.set_exception(excetpion);
				return result_;
			}

			CommandResult data(const std::vector<unsigned char>& data)
			{
				clear();
				result_.set_data(data);
				return result_;
			}			

			CommandResult empty() 
			{
				clear();
				return result_;
			}

			static CommandResult not_valid(dev_module dev)
			{
				std::exception ex("Command check sum not valid");
				return CommandResult(ex, dev, false, false);
			}

			static CommandResult not_valid(dev_module dev)
			{
				std::exception ex("Command check sum not valid");
				return CommandResult(ex, dev, false, false);
			}

			void clear(){
				result_.clear();
			}

		private:
			std::exception       exception_;
			dev_module module_;

			bool ok_;
			bool empty_;

			CommandResult result_;
		};
	}
}

#endif
