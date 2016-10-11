#ifndef IDeviceEngine_Included
#define IDeviceEngine_Included
#include "device_observer.hpp"
#include <contracts/devices/idevice_enumerator.hpp>
#include <contracts/common/ilifecycle.hpp>

namespace contracts
{
	namespace locations
	{
		class IDeviceContainer
		{
		public:
			virtual ~IDeviceContainer(){}

			virtual void stop_all() = 0;

			virtual void add(const std::string& device_name) = 0;

			virtual void remove(const std::string& device_name) = 0;
			virtual bool is_active(const std::string& device_name) = 0;

			virtual const devices::IDeviceEnumerator& device_enumerator() const = 0;
		};

		
		template <typename T>
		class IDeviceEngine : public IDeviceContainer
			                  , common::IModule
		{
		public:
			virtual ~IDeviceEngine() {}		

			virtual void 
				subscribe( std::shared_ptr<devices::IDeviceObserver<T>> observer
				         , const std::string& device_name) = 0;


			virtual void 
				unsubscribe(std::shared_ptr<devices::IDeviceObserver<T>> observer) = 0;
		

			virtual bool 
				has_observer(std::shared_ptr<devices::IDeviceObserver<T>> observer
				            , const std::string& device_name) = 0;

			virtual void unsubscribe_all() = 0;
			
		};

		
	
	}
}

#endif
