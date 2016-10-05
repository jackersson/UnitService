#ifndef AccessDeviceEngine_Included
#define AccessDeviceEngine_Included

#include <boost/asio.hpp>
#include "serial_port_enumerator.hpp"
#include "access_device_listener.hpp"
#include <concurrent_containers.hpp>
#include <contracts/devices/idevice_engine.hpp>
#include <contracts/observers/observable..hpp>

namespace access_device
{
	
	class AccessDeviceEngine : public contracts::locations::IDeviceEngine
	{
	public:
		AccessDeviceEngine()
		{
			AccessDeviceEngine::init();
		}

		~AccessDeviceEngine() {}

		void stop_all() override
		{
			device_enumerator_.stop();
			for ( auto it = devices_.begin(); it != devices_.end(); ++it)
				it->second->stop();
						
			devices_.clear();
		}

		void add(const std::string& device_name) override
		{
			if (device_name == "")
				return;

			if (devices_.contains(device_name))
				return;

			//TODO will be device number
			auto listener = std::make_shared<AccessDeviceListener>(device_name);
			listener->start();
			devices_.insert(device_name, listener);
		}

		void remove(const std::string& device_name) override
		{
			if (device_name == "")
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

		bool is_active(const std::string& device_name) override
		{
			if (device_name == "")
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
		
		
		void Execute(const std::string& device_name
			         , contracts::devices::access_device::lights command)
		{
			if (device_name == "")
				return;

			try
			{
				auto& listener = devices_.find(device_name);
				return listener->execute<>(command);
			}
			catch (std::exception&) {
				//Not implemented
			}
		}
		

		void subscribe( const IAccessDeviceObserver& observer
				          , const std::string& device_name) override
		{
			if (device_name == "")
				return;

			try
			{
				auto& listener = devices_.find(device_name);
		  	return listener->subscribe(observer);
			}
			catch (std::exception&)	{
				//Not implemented
			}
		}

		void unsubscribe(const IAccessDeviceObserver& observer) override
		{
			for ( auto it : devices_) {
				it.second->unsubscribe(observer);
			}		
		}

		bool has_observer( const IAccessDeviceObserver& observer
			               , const std::string& device_name) override
		{
			if (device_name == "")
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

		void unsubscribe_all() override
		{
			for (auto it : devices_) {
				it.second->unsubscribe_all();
			}
		}
		
		const contracts::devices::IDeviceEnumerator& device_enumerator() const override
		{
			return device_enumerator_;
		}
		/*
		void add_range(std::vector<std::string> devices)
		{
			if (devices.size() <= 0)
			{
				stop_all();
				return;
			}

			//auto devices_to_add = devices.// devices.Where(x = > !ContainsKey(x));
			for ( auto it: devices_)
			{
				if ()
			}
			auto devicesToRemove = _devices.Keys.Where(x = > !devices.Contains(x));

			foreach(var deviceName in devicesToAdd.Where(deviceName = > !string.IsNullOrEmpty(deviceName)))
				Add(deviceName);


			foreach(var deviceName in devicesToRemove.Where(deviceName = > !string.IsNullOrEmpty(deviceName)))
				Remove(deviceName);
		}
		*/

		void de_init() override
		{
			stop_all();
			device_enumerator_.stop();
		}

		void init() override
		{
			device_enumerator_.start();
		}

		bool contains_key(std::string key)
		{
			try
			{
				devices_.find(key);
				return true;
			}
			catch (std::exception&)	{
				return false;
			}
		}

		private:

		SerialPortEnumerator device_enumerator_;
		concurrency::containers::ConcurrentMap<std::string, AccessDeviceListenerPtr> devices_;

	};
}

#endif
