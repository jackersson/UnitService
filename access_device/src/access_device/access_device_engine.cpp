#include "access_device/access_device_engine.hpp"

#include <data/models/devices.hpp>

using namespace data_model;
using namespace devices;
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
		for (auto it : devices_)
			it->stop();

		devices_.clear();
	}
	
	void AccessDeviceEngine::add(const DeviceId& device)
	{
		if (device.is_empty())
			return;

		if (get_device_listener(device.serial_number()) != nullptr)
			return;

		auto listener = std::make_shared<AccessDeviceListener>(device, &device_enumerator_);
		listener->start();
		devices_.push_back(listener);
	}

	void AccessDeviceEngine::remove(const DeviceId& device)
	{
		if (device.is_empty())
			return;

		auto listener = get_device_listener(device.serial_number());
		if (listener != nullptr)
		{
			listener->stop();
			devices_.remove(listener);
		}
	}
	
	bool AccessDeviceEngine::is_active(const DeviceId& device)
	{
		if (device.is_empty())
			return false;

		try
		{		
			auto listener = get_device_listener(device.serial_number());
			return listener != nullptr && listener->is_active();
		}
		catch (std::exception&) {
			return false;
		}
	}

	void AccessDeviceEngine::execute( const DeviceId& device
		                              , lights data	) 
	{
		if (device.is_empty())
			return;

		try
		{		
			auto listener = get_device_listener(device.serial_number());
			if (listener != nullptr)
		  	listener->execute<commands::LightCommand>(data);
		}
		catch (std::exception&) {
			//Not implemented
		}
	}

	void AccessDeviceEngine::subscribe(IDeviceObserver<ICommandResultPtr>* observer
		                                , const DeviceId& device)
	{
		if (device.is_empty())
			return;

		try
		{			
			auto listener = get_device_listener(device.serial_number());
			if (listener != nullptr)
			  listener->subscribe(observer);			
		}
		catch (std::exception&) {
			//Not implemented
		}
	}

	void AccessDeviceEngine::unsubscribe(IDeviceObserver<ICommandResultPtr>* observer)
	{
		for (auto it : devices_) {
			it->unsubscribe(observer);
		}
	}

	bool AccessDeviceEngine::has_observer(IDeviceObserver<ICommandResultPtr>* observer
		                                   , const DeviceId& device   )
	{
		if (device.is_empty())
			return false;
		try
		{
			auto listener = get_device_listener(device.serial_number());
			return listener != nullptr && listener->has_observer(observer);
		}
		catch (std::exception&) {
			return false;
		}
	}

	void AccessDeviceEngine::unsubscribe_all()
	{
		for (auto it : devices_) {
			it->unsubscribe_all();
		}
	}
		
	const IDeviceEnumerator& AccessDeviceEngine::enumerator() const
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
		device_enumerator_.start    ();
	}

	AccessDeviceListenerPtr 
		AccessDeviceEngine::get_device_listener(const std::string& device_name)
	{
	  std::lock_guard<std::recursive_mutex> lock(mutex_);
		auto result = std::find_if(devices_.begin(), devices_.end()
			, [&device_name](AccessDeviceListenerPtr listener) -> bool
		{
			return listener->port_name() == device_name;
		});
		return result == devices_.end() ? nullptr : *result;
	}	

	AccessDeviceListenerPtr
		AccessDeviceEngine::get_device_listener(uint16_t device_number)
	{			
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		auto result = std::find_if(devices_.begin(), devices_.end()
			, [&device_number](AccessDeviceListenerPtr listener)
		{
			return listener->id() == device_number;				
		});
		return result == devices_.end() ? nullptr : *result;
	}	
	
	size_t AccessDeviceEngine::size() const
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		return devices_.size();
	}
}

