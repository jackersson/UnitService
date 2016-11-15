#include "access_device/serial_port_enumerator.hpp"

#include <thread>
#include <serial/serial.h>
#include <iostream>

#include <data/models/devices.hpp>

using namespace std::chrono;

namespace access_device
{	
	milliseconds SerialPortEnumerator::delay_ = milliseconds(1000);
	
	bool SerialPortEnumerator::connected(const data_model::DeviceId& device_name) const {
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		return  std::find_if(devices_.begin(), devices_.end(),
			[&device_name](AccessDeviceImplPtr device) -> bool
		{
			return device->id() == device_name.serial_number();
		}) != devices_.end();
	}

	void SerialPortEnumerator::enumerate(std::vector<data_model::DeviceId>& devs) const {
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		for (auto dev : devices_)
		{
			if (dev->device_valid())
				devs.push_back(data_model::DeviceId(dev->port_name(), dev->id()));
		}
	}

	AccessDeviceImplPtr
		SerialPortEnumerator::get_device(const data_model::DeviceId& device_name) const
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		auto res = std::find_if(devices_.begin(), devices_.end(),
			[&device_name](AccessDeviceImplPtr device) -> bool
		{
			return device->device_valid() && device->id() == device_name.serial_number();
		});

		return res == devices_.end() ? nullptr : *res;
	}

	void SerialPortEnumerator::run()
	{
		while (active())
		{
			auto serials = serial::list_ports();
			serials_.clear();
			for (auto pi : serials)
				serials_.push_back(std::string(pi.port));
			update();
			//print();
			std::this_thread::sleep_for(delay_);

			if (cancelation_requested)
				break;
		}
	}

	void SerialPortEnumerator::print()
	{
		std::cout << "****Devices****" << std::endl;
		for (auto device_name : devices_)
			std::cout << device_name->device_valid() << " "
			          << device_name->port_name()    << " " 
			          << device_name->id()           << std::endl;
		std::cout << "****************" << std::endl;
	}

	void SerialPortEnumerator::update()
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		if (devices_.size() > 0)
		{
			devices_.erase( remove_if(devices_.begin(), devices_.end()
				, [&](AccessDeviceImplPtr device_name)
			{
				return find(serials_.begin(), serials_.end(), device_name->port_name())
					== serials_.end();
			}), devices_.end());
		}

		for (auto device_name : serials_)
		{
			if (!contains(device_name))
			{
				auto item = std::make_shared<AccessDeviceImpl>(device_name);
				devices_.push_back(item);
			}
		}			
	}
	
	bool SerialPortEnumerator::contains(const std::string& port_name)
	{
		return std::find_if(devices_.begin(), devices_.end()
			, [&port_name](AccessDeviceImplPtr device) -> bool
		{
			return device->port_name() == port_name;
		}) != devices_.end();
	}
}
