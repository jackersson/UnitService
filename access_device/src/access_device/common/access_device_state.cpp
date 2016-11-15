#include "access_device/common/access_device_state.hpp"

#include <data/models/devices.hpp>

namespace access_device
{
	namespace common {
	
		AccessDeviceState::AccessDeviceState(data_model::DeviceState state)
			: dev_type_(data_model::DeviceType::CardReader)
			, state_   (state)
		{}
	}
}
