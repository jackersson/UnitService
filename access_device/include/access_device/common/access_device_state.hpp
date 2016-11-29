#ifndef AccessDeviceState_Included
#define AccessDeviceState_Included

#include <devices/device_observer.hpp>

namespace access_device
{
	namespace common {

		class AccessDeviceState : public devices::IDeviceState
		{
		public:
			explicit AccessDeviceState(data_model::DeviceState state);
		
			data_model::DeviceType  type() const override	{
				return dev_type_;
			}

			data_model::DeviceState state()  const override	{
				return state_;
			}

		private:
			data_model::DeviceType  dev_type_;
			data_model::DeviceState state_   ;
		};
	}
}

#endif