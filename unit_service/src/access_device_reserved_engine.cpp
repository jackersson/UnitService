#include "access_devices_reserved_engine.hpp"

#include <access_device/access_device_engine.hpp>
#include <data/models/devices.hpp>

AccessDevicesReservedEngine::AccessDevicesReservedEngine(contracts::devices::IDevicesSet* reserved_devices)
	: impl_(std::make_unique<access_device::AccessDeviceEngine>())
	, reserved_devices_(reserved_devices)
{
}

void AccessDevicesReservedEngine::remove(const std::string& device_name)  {
	if (reserved_devices_ == nullptr
		|| !reserved_devices_->contains(device_name
			, data_model::DeviceType::CardReader))
		return impl_->remove(device_name);
}
