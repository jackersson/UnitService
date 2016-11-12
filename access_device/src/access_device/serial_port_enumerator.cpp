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
		return find(devices_.begin(), devices_.end(), device_name)
			!= devices_.end();
	}

	std::vector<data_model::DeviceId> SerialPortEnumerator::devices() const {
		return devices_;
	}

	void SerialPortEnumerator::run()
	{
		while (active())
		{
			auto serials = serial::list_ports();
			serials_.clear();
			for (auto pi : serials)
				serials_.push_back(data_model::DeviceId(pi.port));
			update();
			//print();
			std::this_thread::sleep_for(delay_);

			if (cancelation_requested)
				break;
		}
	}

	void SerialPortEnumerator::print()
	{
		std::cout << std::endl;
		for (auto device_name : serials_)
			std::cout << device_name.serial_number() << std::endl;
		std::cout << std::endl;
	}

	void SerialPortEnumerator::update()
	{
		if (devices_.size() > 0)
		{
			auto predicate = [this](const data_model::DeviceId& device_name) {
				return find(serials_.begin(), serials_.end(), device_name)
					!= serials_.end();
			};

			devices_.erase(std::remove_if(devices_.begin(), devices_.end(), predicate)
				, devices_.end());
		}

		for (auto device_name : serials_)
		{
			if (!connected(device_name))
				devices_.push_back(device_name);
		}
	}
}
