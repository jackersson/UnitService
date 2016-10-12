#ifndef AccessDeviceEngine_Included
#define AccessDeviceEngine_Included

#include <boost/asio.hpp>
#include "serial_port_enumerator.hpp"
#include "access_device_listener.hpp"
#include <concurrent_containers.hpp>
#include <contracts/devices/access_device/iaccess_device_engine.hpp>

namespace access_device
{
	
	class AccessDeviceEngine 
		: public contracts::devices::access_device::IAccessDeviceEngine
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

		void execute(const std::string& device_name
			, contracts::devices::access_device::lights data
			= contracts::devices::access_device::lNone) override
		{
			if (device_name == "")
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
			

		void subscribe( IAccessDeviceObserverPtr observer
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

		void unsubscribe(IAccessDeviceObserverPtr observer) override
		{
			for ( auto it : devices_) {
				it.second->unsubscribe(observer);
			}		
		}

		bool has_observer( IAccessDeviceObserverPtr observer
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
			
			return false;

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
		AccessDeviceEngine(const AccessDeviceEngine& other) = delete;
		AccessDeviceEngine& operator=(const AccessDeviceEngine&) = delete;

		SerialPortEnumerator device_enumerator_;
		concurrent::containers::ConcurrentMap<std::string, AccessDeviceListenerPtr> devices_;

	};
	
}

#endif
