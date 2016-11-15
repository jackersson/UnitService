#ifndef AccessDeviceEngine_Included
#define AccessDeviceEngine_Included

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
		AccessDeviceEngine ();
		~AccessDeviceEngine();

		void stop_all() override;

		void add    (const data_model::DeviceId& device) override;
		void remove (const data_model::DeviceId& device) override;
		
		bool is_active(const data_model::DeviceId& device_name) override;

		void execute(const data_model::DeviceId&
			, contracts::devices::access_device::lights data
			= contracts::devices::access_device::lNone) override;
			

		void subscribe   ( IAccessDeviceObserver*      observer
			               , const data_model::DeviceId& device_name) override;

		void unsubscribe (IAccessDeviceObserver* observer) override;

		bool has_observer( IAccessDeviceObserver* observer
			               , const data_model::DeviceId& device_name) override;

		void unsubscribe_all() override;		

		void de_init() override;
		void init   () override;


		void enumerate_devices(std::vector<data_model::DeviceId>& devs) override;


	private:
		mutable std::recursive_mutex mutex_;
		
		AccessDeviceListenerPtr get_device_listener(uint16_t number);
		AccessDeviceListenerPtr get_device_listener(const std::string& name);
			
			
		AccessDeviceEngine(const AccessDeviceEngine& other) = delete;
		AccessDeviceEngine& operator=(const AccessDeviceEngine&) = delete;

		SerialPortEnumerator device_enumerator_;

		concurrent::containers::ConcurrentVector<AccessDeviceListenerPtr> devices_;
		
	};
	
}

#endif
