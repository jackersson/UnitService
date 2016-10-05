#include "directshow_device_enumerator.hpp"

#include <thread>
#include <iostream>
#include <algorithm>
#include <containers_utils.hpp>
#include <platform.hpp>

namespace directshow_device
{
  std::chrono::milliseconds delay = std::chrono::milliseconds(1000);
	ca::Capture capture;

	bool DirectshowDeviceEnumerator::connected(const std::string& device_name) const  {
		return utils::containers::contains<std::string>(devices_, device_name);
	}

	std::vector<std::string> DirectshowDeviceEnumerator::devices() const {
		return devices_;
	}

	void DirectshowDeviceEnumerator::run() 
	{
		while (active())
		{
			enumerate();
			update();
			print();
			std::this_thread::sleep_for(delay);
		}
	}

	void DirectshowDeviceEnumerator::enumerate()
	{		
		actual_.clear();
		auto devices = capture.getDevices();
		for (auto dev : devices)
			actual_.push_back(dev.name);
	}

	void DirectshowDeviceEnumerator::print()
	{
		std::cout << std::endl;
		for (auto device_name : actual_)
			std::cout << device_name << std::endl;
		std::cout << std::endl;
	}
	
	void DirectshowDeviceEnumerator::update()
	{
		if (devices_.size() > 0)
		{

			auto predicate = [this](const std::string& device_name) {
				return utils::containers::contains(actual_, device_name);
			};

			devices_.erase(std::remove_if(devices_.begin(), devices_.end(), predicate)
				, devices_.end());
		}

		for (auto device_name : actual_)
		{
			if (!utils::containers::contains(devices_, device_name))
				devices_.push_back(device_name);
		}
	}
}

