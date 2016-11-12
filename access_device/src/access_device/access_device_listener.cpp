#include "access_device/access_device_listener.hpp"

#include <data/models/devices.hpp>
#include <contracts/devices/idevice_info.hpp>

using namespace data_model;
using namespace contracts::devices::access_device;

namespace access_device
{
	AccessDeviceListener::AccessDeviceListener(const DeviceId& device_name
	, contracts::devices::IDeviceInfo<DeviceId>* device_holder)
		: factory_(device_name.serial_number())
		, device_name_(std::make_unique<DeviceId>(device_name))
		, serial_port_()
		, need_to_ask_buttons_(false)
		, need_to_recover_(false)
		, devices_holder_(device_holder)
	{

	}

	AccessDeviceListener::~AccessDeviceListener() {
		AccessDeviceListener::stop();
	}

	void AccessDeviceListener::stop()
	{
		Threadable::stop();
		factory_.reset(serial_port_);
		serial_port_.close();			
	}
	
	void AccessDeviceListener::run() 
	{
		open();

		while (active())
		{
			auto command = dequeue();
			if (command != nullptr)
			{
				std::this_thread::sleep_for(delay_between_ask_device_);
				auto execution_result = command->execute(serial_port_);

				if (execution_result->ok())
				{
					if (!execution_result->empty())
						on_next(execution_result);

					if (need_to_recover_)
						unlock();
				}
				else
					handle_exception(execution_result->exception());
			}

			if (cancelation_requested)
				break;
		}

		clear();
	}

	void AccessDeviceListener::handle_exception(const std::exception& ex)
	{
		if (!is_active())
		{
			on_error(ex);
			std::this_thread::sleep_for(delay_between_ask_device_);
			open();
		}
		else if (ex.what() != "Undefined exception")
		{
			on_error(ex);
			lock();
		}
		else
		{
			on_state(Active);
			unlock();
		}
	}

	void AccessDeviceListener::lock()
	{
		need_to_recover_ = true;
	}

	void AccessDeviceListener::unlock()
	{
		if (!need_to_recover_)
			return;
		execute<commands::LightCommandImpl>(lRedMain);
		need_to_recover_ = false;
	}

	void AccessDeviceListener::execute(core::IExecutableCommandPtr command)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		commands_.push(command);
	}
			
	void AccessDeviceListener::open()
	{
		if (serial_port_.is_open())
			return;

		try
		{
			DeviceId di;
			if (!devices_holder_->try_get_info(*device_name_, di))
				return;

			serial_port_.open(di.name(), BAUD_RATE);
			//TODO const
			serial_port_.set_timeout(boost::posix_time::millisec(100));
			if (!factory_.reset(serial_port_)) //TODO maybe to log
				std::cout << "Can't reset device" << std::endl; 

			execute<commands::LightCommandImpl>(lRedMain);

			on_state(Active);

			need_to_recover_ = false;
		}
		catch (std::exception& exception) {
			on_error(exception);
		}
	}	

	core::IExecutableCommandPtr AccessDeviceListener::dequeue()
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		while (commands_.empty())
		{
			std::this_thread::sleep_for(delay_between_ask_device_);
			what_with_access_device();
		}
		auto ptr = commands_.front();
		commands_.pop();
		return ptr;
	}

	void AccessDeviceListener::what_with_access_device()
	{
		if (need_to_ask_buttons_)
			execute<commands::ButtonCommandImpl>();
		else
			execute<commands::DallasCommandImpl>();
		need_to_ask_buttons_ = !need_to_ask_buttons_;
	}

	void
		AccessDeviceListener::on_error(const std::exception& exception)
	{
		contracts::devices::DeviceException
			device_exception(exception.what(), CardReader);

		for (auto observer : observers_)
			observer->on_error(device_exception);
	}

	void
		AccessDeviceListener::on_state(DeviceState state)
	{
		common::AccessDeviceState ac_state(state);
		for (auto observer : observers_)
			observer->on_state(ac_state);
	}

	void
		AccessDeviceListener::on_next(ICommandResultPtr data)
	{
		for (auto observer : observers_)
			observer->on_next(*data.get());
	}
}


