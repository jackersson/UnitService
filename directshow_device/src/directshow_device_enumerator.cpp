#include "directshow_device_enumerator.hpp"

#include <thread>
#include <iostream>
#include <algorithm>
#include <platform.hpp>

namespace directshow_device
{
  std::chrono::milliseconds delay = std::chrono::milliseconds(1000);
	ca::Capture capture(nullptr, nullptr);

	bool contains( const std::vector<DirectShowDeviceInfo>& items
		           , const std::string& target)
	{
		for (auto item : items)
		{
			if (item.name() == target)
				return true;
		}
		return false;
	}

	bool DirectshowDeviceEnumerator::connected(const std::string& device_name) const  {
		return find(devices_.begin(), devices_.end(), device_name) != devices_.end();
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
			//print();
			std::this_thread::sleep_for(delay);

			if (cancelation_requested)
				break;
		}
	}

	void DirectshowDeviceEnumerator::enumerate()
	{		
		std::lock_guard<std::recursive_mutex> lock(mutex_);

		actual_.clear();
		auto devices = capture.getDevices();
		for (auto dev : devices)
		{
			DirectShowDeviceInfo info(dev.name, dev.index);
			actual_.push_back(info);
		}
	}

	void DirectshowDeviceEnumerator::print()
	{
		std::cout << std::endl;
		for (auto device : actual_)
			std::cout << device.name() << std::endl;
		std::cout << std::endl;
	}
	
	void DirectshowDeviceEnumerator::update()
	{
		if (devices_.size() > 0)
		{
			auto predicate = [this](const std::string& device_name) {
				return contains(actual_, device_name);
			};

			devices_.erase(std::remove_if(devices_.begin(), devices_.end(), predicate)
				, devices_.end());
		}

		for (auto device_name : actual_)
		{
			auto dev_name = device_name.name();
			if (!connected(dev_name))
				devices_.push_back(dev_name);
		}
	}

	bool 
		DirectshowDeviceEnumerator::try_get_info( const std::string& device_name
			                                      , DirectShowDeviceInfo& info)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		for (auto device : actual_)
		{
			if (device.name() == device_name)
			{
				info = device;
				auto caps = capture.getCapabilities(device.id());
				std::vector<Capability> caps_ok;
				for (auto cap : caps)
				{
					if  (cap.pixel_format == CA_MJPEG)
					caps_ok.push_back(Capability(cap.width, cap.height));
				}

				info.set_capabilities(caps_ok);
				return true;
			}
		}

		return false;
	}

}

