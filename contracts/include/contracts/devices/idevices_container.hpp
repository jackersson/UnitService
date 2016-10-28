#ifndef IDevicesContainer_Included
#define IDevicesContainer_Included

#include <contracts/common/ilifecycle.hpp>
#include <contracts/devices/access_device/iaccess_device_engine.hpp>

namespace contracts
{
	namespace devices
	{
		class IDevicesSet
		{
		public:
			virtual ~IDevicesSet() {}

			virtual bool contains( const std::string& device_name
				                   , DataTypes::DeviceType dev_type) const = 0;
		};

		class IDevicesContainer : public common::IModule
			                      , public IDevicesSet
		{
		public:
			virtual access_device::IAccessDeviceEngine* access_device_engine() = 0;

			virtual void enumerate(DataTypes::Devices&) const = 0;
		};

	}
}

#endif
