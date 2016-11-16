#include "directshow_device_engine.hpp"

#include <data/models/devices.hpp>

#include "directshow_device_listener.hpp"

using namespace contracts::devices;
using namespace video_device;

namespace directshow_device
{	
	DirectShowDeviceEngine::DirectShowDeviceEngine() {
		DirectShowDeviceEngine::init();
	}

	DirectShowDeviceEngine::~DirectShowDeviceEngine() {
		DirectShowDeviceEngine::de_init();
	}

	void DirectShowDeviceEngine::stop_all() 
	{
		for (auto it = devices_.begin(); it != devices_.end(); ++it)
			it->second->stop();

		devices_.clear();
	}

	void DirectShowDeviceEngine::add(const data_model::DeviceId& device) 
	{
		if (device.is_empty())
			return;

		if (contains_key(device))
			return;
		const auto& device_name = device.name();

		auto listener = std::make_shared<DirectshowDeviceListener>(device_name
			                                    , &device_enumerator_);
		listener->start();
		devices_.insert(device_name, listener);
	}

	void DirectShowDeviceEngine::remove(const data_model::DeviceId& device) 
	{
		if (device.is_empty())
			return;

		try
		{
			const auto& device_name = device.name();
			const auto& listener = devices_.find(device_name);
			listener->stop();
			devices_.remove(device_name);
		}
		catch (std::exception&) {
			//Not implemented
		}
	}

	bool DirectShowDeviceEngine::is_active(const data_model::DeviceId& device_name) 
	{
		if (device_name.is_empty())
			return false;

		try
		{
			auto& listener = devices_.find(device_name.name());
			return listener->active();
		}
		catch (std::exception&) {
			return false;
		}
	}

	void DirectShowDeviceEngine::subscribe(IDeviceObserver<IStreamData>* observer
		, const data_model::DeviceId& device_name) 
	{
		if (device_name.is_empty())
			return;

		try
		{
			auto& listener = devices_.find(device_name.name());
			listener->subscribe(observer);
		}
		catch (std::exception&) {
			//Not implemented
		}
	}

	void DirectShowDeviceEngine::unsubscribe(IDeviceObserver<IStreamData>* observer)
	{
		for (auto it : devices_) {
			it.second->unsubscribe(observer);
		}
	}

	bool DirectShowDeviceEngine::has_observer(IDeviceObserver<IStreamData>* observer
		, const data_model::DeviceId& device_name) 
	{
		if (device_name.is_empty())
			return false;
		try
		{
			auto& listener = devices_.find(device_name.name());
			return listener->has_observer(observer);
		}
		catch (std::exception&) {
			return false;
		}
	}

	void DirectShowDeviceEngine::unsubscribe_all()
	{
		for (auto it : devices_) {
			it.second->unsubscribe_all();
		}
	}
	
	void DirectShowDeviceEngine::de_init()
	{
		stop_all();
		device_enumerator_.stop();
	}

	void DirectShowDeviceEngine::init()
	{
		device_enumerator_.start();
	}

	bool DirectShowDeviceEngine::contains_key(const data_model::DeviceId& key)
	{
		try
		{
			devices_.find(key.name());
			return true;
		}
		catch (std::exception&) {
			return false;
		}
	}

	const contracts::devices::IDeviceEnumerator& 
		    DirectShowDeviceEngine::enumerator() const
	{
		return device_enumerator_;
	}

	size_t DirectShowDeviceEngine::size() const
	{
		return devices_.size();
	}
}
