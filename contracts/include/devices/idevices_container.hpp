#ifndef IDevicesContainer_Included
#define IDevicesContainer_Included

#include <common/ilifecycle.hpp>
#include "devices/access_device/iaccess_device_engine.hpp"
#include "devices/video_device/ivideo_engine.hpp"

namespace data_model
{
	enum  DeviceType;
	class Devices   ;
	class DeviceId  ;
}


namespace devices
{
	class IDevicesSet
	{
	public:
		virtual ~IDevicesSet() {}

		virtual bool contains( const data_model::DeviceId& device_name
			                   , data_model::DeviceType dev_type) const = 0;
	};

	class IDevicesContainer : public contracts::common::IModule
		                      , public IDevicesSet
	{
	public:
		virtual access_device::IAccessDeviceEngine* access_device_engine() = 0;
		virtual video_device::IVideoEngine*     
			          directshow_device_engine() = 0;

		virtual void enumerate( data_model::Devices&
			                    , data_model::DeviceType type) const = 0;
	};

}


#endif
