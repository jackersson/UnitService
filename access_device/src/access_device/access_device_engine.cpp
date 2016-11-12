#include "access_device/access_device_engine.hpp"

#include <data/models/devices.hpp>

using namespace contracts::devices;
using namespace contracts::devices::access_device;
using namespace data_model;

namespace access_device
{	
	AccessDeviceEngine::AccessDeviceEngine()
	{
		AccessDeviceEngine::init();
	}

	AccessDeviceEngine::~AccessDeviceEngine() {
		AccessDeviceEngine::de_init();
	}

	void AccessDeviceEngine::stop_all() 
	{
		for (auto it = devices_.begin(); it != devices_.end(); ++it)
			it->second->stop();

		devices_.clear();
	}

	void AccessDeviceEngine::add(const DeviceId& device_name) 
	{
		if (device_name.is_empty())
			return;

		if (devices_.contains(device_name))
			return;

		auto listener = std::make_shared<AccessDeviceListener>(device_name);
		listener->start();
		devices_.insert(device_name, listener);
	}

	void AccessDeviceEngine::remove(const DeviceId& device_name)
	{
		if (device_name.is_empty())
			return;

		try
		{
			const auto& listener = devices_.find(device_name);
			listener->stop();
			devices_.remove(device_name);
		}
		catch (std::exception&) {
			//Not implemented
		}
	}

	bool AccessDeviceEngine::is_active(const DeviceId& device_name)
	{
		if (device_name.is_empty())
			return false;

		try
		{
			auto& listener = devices_.find(device_name);
			return listener->active();
		}
		catch (std::exception&) {
			return false;
		}
	}

	void AccessDeviceEngine::execute( const DeviceId& device_name
		                              , lights data	= lNone) 
	{
		if (device_name.is_empty())
			return;

		try
		{
			auto& listener = devices_.find(device_name);
			return listener->execute<commands::LightCommandImpl>(data);
		}
		catch (std::exception&) {
			//Not implemented
		}
	}


	void AccessDeviceEngine::subscribe( IAccessDeviceObserver* observer
		                                , const DeviceId& device_name)
	{
		if (device_name.is_empty())
			return;

		try
		{
			auto& listener = devices_.find(device_name);
			return listener->subscribe(observer);
		}
		catch (std::exception&) {
			//Not implemented
		}
	}

	void AccessDeviceEngine::unsubscribe(IAccessDeviceObserver* observer) 
	{
		for (auto it : devices_) {
			it.second->unsubscribe(observer);
		}
	}

	bool AccessDeviceEngine::has_observer( IAccessDeviceObserver* observer
		                                   , const DeviceId& device_name   )
	{
		if (device_name.is_empty())
			return false;
		try
		{
			auto& listener = devices_.find(device_name);
			return listener->has_observer(observer);
		}
		catch (std::exception&) {
			return false;
		}
	}

	void AccessDeviceEngine::unsubscribe_all()
	{
		for (auto it : devices_) {
			it.second->unsubscribe_all();
		}
	}

	const IDeviceEnumerator& AccessDeviceEngine::device_enumerator() const
	{
		return device_enumerator_;
	}

	void AccessDeviceEngine::de_init()
	{
		stop_all();
		device_enumerator_.stop();
	}

	void AccessDeviceEngine::init()
	{
		device_enumerator_.start();
	}

	bool AccessDeviceEngine::contains_key(const DeviceId& device_name)
	{
		try
		{
			devices_.find(device_name);
			return true;
		}
		catch (std::exception&) {
			return false;
		}
	}	
}

