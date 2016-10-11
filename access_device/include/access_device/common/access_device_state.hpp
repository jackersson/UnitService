#ifndef AccessDeviceState_Included
#define AccessDeviceState_Included
#include <contracts/devices/device_observer.hpp>
#include <datatypes/devices.pb.h>

namespace access_device
{
	namespace common {

		class AccessDeviceState : public contracts::devices::IDeviceState
		{
		public:
			explicit AccessDeviceState(DataTypes::DeviceState state)	
				: dev_type_(DataTypes::DeviceType::CardReader)
				, state_(state)
			{}

			DataTypes::DeviceType  type() const override
			{
				return dev_type_;
			}

			DataTypes::DeviceState state()  const override
			{
				return state_;
			}

		private:
			DataTypes::DeviceType  dev_type_;
			DataTypes::DeviceState state_   ;
		};
	}
}

#endif