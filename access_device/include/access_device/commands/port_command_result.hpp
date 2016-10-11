#ifndef CommandResultPool_Included
#define CommandResultPool_Included

#include <vector>
#include <contracts/devices/access_device/access_device_types.hpp>
#include "command_result.hpp"

namespace access_device
{
	namespace commands {
		class PortCommandResult
		{
			typedef contracts::devices::access_device::access_device_module dev_module;

		public:
			explicit PortCommandResult(rs232::port_command command	)
				: result_(std::make_shared<CommandResult>())
			{
				result_->set_module(to_device_module(command));
			}

			//TODO to utils
			static dev_module to_device_module(rs232::port_command command)
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

			std::shared_ptr<CommandResult> not_valid() const
			{
				clear();
				std::exception ex("Command check sum not valid");
				result_->set_exception(ex);
				return result_;
			}

			std::shared_ptr<CommandResult> exception(const std::exception& excetpion) const
			{
				clear();
				result_->set_exception(excetpion);
				return result_;
			}

			std::shared_ptr<CommandResult> data(const std::vector<unsigned char>& data) const
			{
				clear();
				result_->set_data(data);
				return result_;
			}			

			std::shared_ptr<CommandResult> empty() const
			{
				clear();
				return result_;
			}
						
			static std::shared_ptr<CommandResult> not_valid(dev_module dev)
			{
				std::exception ex("Command check sum not valid");
				return std::make_shared<CommandResult>(dev, ex, false, false);
			}

			void clear() const {
				result_->clear();
			}

		private:
			std::shared_ptr<CommandResult> result_;
		};
	}
}

#endif
