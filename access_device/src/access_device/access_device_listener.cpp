#include <access_device/access_device_listener.hpp>

namespace access_device
{
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
			on_state(data_model::DeviceState::Active);
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
		execute<commands::LightCommandImpl>(contracts::devices::access_device::lights::lRedMain);
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
			serial_port_.open(device_name_, BAUD_RATE);
			serial_port_.set_timeout(boost::posix_time::millisec(100));
			if (!factory_.reset(serial_port_))
				std::cout << "Can't reset device" << std::endl; //TODO maybe to log

			execute<commands::LightCommandImpl>(contracts::devices::access_device::lights::lRedMain);

			on_state(data_model::DeviceState::Active);

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
}


