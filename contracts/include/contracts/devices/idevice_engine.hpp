#ifndef IDeviceEngine_Included
#define IDeviceEngine_Included

#include "device_observer.hpp"
#include <contracts/devices/idevice_enumerator.hpp>
#include <common/ilifecycle.hpp>

namespace data_model
{
	class DeviceId;
}

namespace contracts
{
	namespace locations
	{
		class IDeviceContainer
		{
		public:
			virtual ~IDeviceContainer(){}

			virtual void stop_all() = 0;

			virtual void add      (const data_model::DeviceId& device_name) = 0;												    
			virtual void remove   (const data_model::DeviceId& device_name) = 0;
			virtual bool is_active(const data_model::DeviceId& device_name) = 0;

			virtual void enumerate_devices(std::vector<data_model::DeviceId>& devs) = 0;
		};
		
		template <typename T>
		class IDeviceEngine : public IDeviceContainer
			                  , public common::IModule
		{
		public:
			virtual ~IDeviceEngine() {}		

			virtual void 
				subscribe( devices::IDeviceObserver<T>* observer
				         , const data_model::DeviceId& device_name) = 0;


			virtual void 
				unsubscribe(devices::IDeviceObserver<T>* observer) = 0;
		

			virtual bool 
				has_observer(devices::IDeviceObserver<T>* observer
				            , const data_model::DeviceId& device_name) = 0;

			virtual void unsubscribe_all() = 0;
			
		};

		
	
	}
}

#endif
