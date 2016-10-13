#ifndef IDevicesContainer_Included
#define IDevicesContainer_Included

#include <contracts/common/ilifecycle.hpp>
#include <contracts/devices/access_device/iaccess_device_engine.hpp>

namespace contracts
{
	namespace devices
	{
		class IDevicesContainer : public common::IModule
		{
		public:
			virtual access_device::IAccessDeviceEnginePtr access_device_engine() = 0;

			virtual void enumerate(DataTypes::Devices&) const = 0;
		};

	}
}

#endif
