#include "access_device/access_device_listener.hpp"

#include <data/models/devices.hpp>
#include "access_device/common/access_device_state.hpp"

#include <tbb/task_group.h>


using namespace data_model;
using namespace contracts::devices::access_device;
using namespace std::chrono;

namespace access_device
{
	milliseconds 
		AccessDeviceListener::read_write_timeout_	      = milliseconds(1000);
	milliseconds 
		AccessDeviceListener::delay_between_ask_device_ = milliseconds(100);
	
	milliseconds reconnection_delay_ = milliseconds(2000);

	AccessDeviceListener::AccessDeviceListener
	  ( const DeviceId& device_id
		, contracts::devices::IDeviceInfo<AccessDeviceImplPtr>* devices)
		: device_number_(std::make_unique<DeviceId>(device_id))
		, devices_(devices)
		, need_to_ask_buttons_(false)
		, need_to_recover_(false)
		, next_busy_(false)
		, tasks_(std::make_unique<tbb::task_group>())
	{		
		start();
	}

	AccessDeviceListener::~AccessDeviceListener() {
		AccessDeviceListener::stop();
	}

	void AccessDeviceListener::stop()
	{
		if (!cancelation_requested)
			on_state(Stopped);
		clear();
		Threadable::stop();
		if (access_device_impl_ != nullptr)
	  	access_device_impl_->de_init();		
		tasks_->cancel();
	}
		
	void AccessDeviceListener::run() 
	{		
		init_session();
		
		while (active())
		{
			auto command = dequeue();
			if (command != nullptr)
			{				
				try
				{
					auto result = access_device_impl_->execute(command);
					if (result->is_valid() && !result->is_empty())
					{
						tasks_->run([this, result]()
						{
							on_next(result);
						});
					}

					if (need_to_recover_)
						unlock();
				}
				catch (std::exception& ex) {
					handle_exception(ex);
				}				
			}

			if (cancelation_requested)
				break;
		}
		stop();
	}

	//TODO think about smarter way
	void AccessDeviceListener::handle_exception(const std::exception& ex)
	{
		if (is_active() && ex.what() == "Undefined exception")
			return;

		on_error(ex);
		std::this_thread::sleep_for(reconnection_delay_);
		access_device_impl_ = devices_->get_device(*device_number_);
		init_session();
		lock();
	}

	bool AccessDeviceListener::is_active() const
	{
		return access_device_impl_ != nullptr && access_device_impl_->is_open() && active();
	}

	uint16_t AccessDeviceListener::id() const{
		return device_number_->serial_number();
	}

	const std::string& AccessDeviceListener::port_name() const {
		if (access_device_impl_ == nullptr)
			return device_number_->name();
		return access_device_impl_->port_name();
	}

	void AccessDeviceListener::lock()
	{
		need_to_recover_ = true;
	}

	void AccessDeviceListener::unlock()
	{
		if (!need_to_recover_)
			return;
		activate();
	}

	void AccessDeviceListener::execute(core::IExecutableCommandPtr command)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		commands_.push(command);
	}

	bool AccessDeviceListener::open()
	{
		if (access_device_impl_ == nullptr)
			return false;
		try
		{
			return access_device_impl_->open();
		}
		catch (std::exception&) {
			return false;
		}
	}		

	void AccessDeviceListener::init_session()
	{
		if (!open())
		{
			on_state(Error);
			return;
		}
		if (access_device_impl_ != nullptr && !access_device_impl_->reset()) //TODO maybe to log
			std::cout << "Can't reset device" << std::endl;

		activate();			
	}	

	void AccessDeviceListener::activate()
	{
		execute<commands::LightCommand>(lRedMain);
		on_state(Active);
		need_to_recover_ = false;
	}

	core::IExecutableCommandPtr AccessDeviceListener::dequeue()
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		while (commands_.empty() && !cancelation_requested)
		{
			std::this_thread::sleep_for(delay_between_ask_device_);
			what_with_access_device();		
		}
		if (commands_.empty())
			return nullptr;

		auto ptr = commands_.front();
		commands_.pop();
		return ptr;
	}

	void AccessDeviceListener::what_with_access_device()
	{		
		if (need_to_ask_buttons_)
			 execute<commands::ButtonCommand>();
		else
			execute<commands::DallasCommand>();
		need_to_ask_buttons_ = !need_to_ask_buttons_;
	}

	void
		AccessDeviceListener::on_error(const std::exception& exception)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		contracts::devices::DeviceException
			device_exception(exception.what(), CardReader);

		for (auto observer : observers_)
			observer->on_error(device_exception);
	}

	void
		AccessDeviceListener::on_state(DeviceState state)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		common::AccessDeviceState ac_state(state);
		for (auto observer : observers_)
			observer->on_state(ac_state);
	}

	void
		AccessDeviceListener::on_next(ICommandResultPtr data)
	{		
		//if (next_busy_)
		//{
			//std::cout << "Next busy : rejected";
			//return;
		//}

		if (data->device_module() == Dallas || data->device_module() == Buttons)
		{			
			//next_busy_ = true;
			std::lock_guard<std::recursive_mutex> lock(mutex_);
		  for (auto observer : observers_)
		  {
				tasks_->run([data, observer]()
		  	{
		  		observer->on_next(*data.get());		  
		  	});
		  }			
			//next_busy_ = false;
		}
	}
}


