#include "directshow_device_enumerator.hpp"

#include <thread>
#include <iostream>
#include <algorithm>
#include <platform.hpp>
#include <data/models/devices.hpp>

#include "directshow_device_info.hpp"


namespace video_device
{
	DirectshowDeviceEnumerator::DirectshowDeviceEnumerator()
	{}

	DirectshowDeviceEnumerator::~DirectshowDeviceEnumerator(){
		Threadable::stop();
	}

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

	bool DirectshowDeviceEnumerator::connected
	           (const data_model::DeviceId& device_name) const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return  std::find_if(devices_.begin(), devices_.end(),
			[&device_name](const std::string& device) -> bool
		{
			return device == device_name.name();
		}) != devices_.end();
	}

	void DirectshowDeviceEnumerator::enumerate
	           (std::vector<data_model::DeviceId>& devs) const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		for (const auto& dev : devices_)					
			devs.push_back(data_model::DeviceId(dev));		
	}

	bool DirectshowDeviceEnumerator::connected(const std::string& device_name) const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return do_connected(device_name);
	}

	bool DirectshowDeviceEnumerator::do_connected(const std::string& device_name) const
	{
		return find(devices_.begin(), devices_.end(), device_name) != devices_.end();
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
		std::lock_guard<std::mutex> lock(mutex_);

		actual_.clear();
		auto devices = capture.getDevices();
		for (const auto& dev : devices)
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
		std::lock_guard<std::mutex> lock(mutex_);

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
			if (!do_connected(dev_name))
				devices_.push_back(dev_name);
		}
	}


	DirectShowDeviceInfo DirectshowDeviceEnumerator::get_device
	               (const data_model::DeviceId& device_name) const
	{
		throw std::exception("not implemented");
	}

	bool 
		DirectshowDeviceEnumerator::try_get_device( const data_model::DeviceId& device_name
			                                        , DirectShowDeviceInfo& info) const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		for (auto device : actual_)
		{
			if (device.name() == device_name.name())
			{
				info = device;
				auto caps = capture.getCapabilities(device.id());
				std::vector<Capability> caps_ok;
				for (const auto& cap : caps)
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

