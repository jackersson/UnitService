#ifndef AccessDeviceEngine_Included
#define AccessDeviceEngine_Included

#include <boost/asio.hpp>
#include "serial_port_enumerator.hpp"
#include "access_device_listener.hpp"
#include "concurrent_map.hpp"

namespace access_device
{
	class AccessDeviceEngine
	{
	public:
		AccessDeviceEngine()
		{			
			init();
		}

		~AccessDeviceEngine() {}

		void stop_all()
		{
			device_enumerator_.stop();
			for ( auto it = devices_.begin(); it != devices_.end(); ++it)
				it->second->stop();
						
			devices_.clear();
		}

		void add(const std::string& device_name)
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

		void remove(const std::string& device_name)
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

		bool is_active(const std::string& device_name)
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
		/*
		void Execute(const std::string& device_name, lights command)
		{
			if (device_name == "")
				return;

			AccessDeviceListener listener;
			if (devices_.find(device_name, listener))
				listener.execute<LightCommandImpl>(command);
		}

		void subscribe(IDeviceObserver<ICommandResult> observer, std::string device_name)
		{
			if (device_name == "")
				return;

			AccessDeviceListener listener;
			if (devices_.find(device_name, listener))
				listener.subscribe(observer);
		}

		void unsubscribe(IDeviceObserver<ICommandResult> observer)
		{
			foreach(var listener in _devices.Select(par = > par.Value).Where(listener = > listener.HasObserver(observer)))
				listener.Unsubscribe(observer);
		}

		bool has_observer(IDeviceObserver<ICommandResult> observer, std::string device_name)
		{
			if (device_name == "")
				return false;

			AccessDeviceListener listener;
			return devices_.find(device_name, listener) && listener.has_observer(observer);
		}
		*/
		const contracts::devices::IDeviceEnumerator& device_enumerator() const
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

		void de_init()
		{
			stop_all();
			device_enumerator_.stop();
		}

		void init()
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

		serial_port::serial_port_enumerator device_enumerator_;
		utils::ConcurrentMap<std::string, AccessDeviceListenerPtr> devices_;

	};
}

#endif
