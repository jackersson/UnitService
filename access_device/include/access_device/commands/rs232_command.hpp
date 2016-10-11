#ifndef Rs232Command_Included
#define Rs232Command_Included

#include "executable_command_base.hpp"
#include <access_device/core/iexecutable_command.hpp>
#include "command_impl.hpp"

namespace access_device
{
	namespace commands
	{
		class AccessDeviceCommand : public core::ICommandContext
		{
		public:
			explicit AccessDeviceCommand(std::shared_ptr<ExecutableCommandBase> command)
				: command_(command), device_number_(0), data_(0)
			{}

			bool reset(TimeoutSerial& sp) override
			{
				return command_->reset(sp, device_number_);
			}

			rs232::port_command id() const override
			{
				return command_->id();
			}

			contracts::devices::access_device::ICommandResultPtr 
			execute(TimeoutSerial& sp) override
			{
				return command_->execute(sp, device_number_, data_);
			}

			void set_device_number(uint16_t dev_number) override {
				device_number_ = dev_number;
			}

			void set_data(uint32_t data) override {
				data_ = data;
			}

			uint16_t device_number() const	override {
				return device_number_;
			}

			uint32_t data() const override	{
				return data_;
			}

		private:
			std::shared_ptr<ExecutableCommandBase> command_;
			uint16_t device_number_;
			uint32_t data_;
		};

		class LightCommandImpl : public AccessDeviceCommand
		{
		public:
			 LightCommandImpl(): AccessDeviceCommand(std::make_shared<LightCommand>()){}			
		};

		class ButtonCommandImpl : public AccessDeviceCommand
		{
		public:
			ButtonCommandImpl() : AccessDeviceCommand(std::make_shared<ButtonCommand>()) {}
		};

		class DallasCommandImpl : public AccessDeviceCommand
		{
		public:
			DallasCommandImpl() : AccessDeviceCommand(std::make_shared<DallasCommand>()) {}
		};

	}
}

#endif
