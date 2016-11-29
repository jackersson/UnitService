#include "access_devices_reserved_engine.hpp"

#include <access_device/access_device_engine.hpp>
#include <data/models/devices.hpp>

using namespace data_model;

AccessDevicesReservedEngine::AccessDevicesReservedEngine(devices::IDevicesSet* reserved_devices)
	: impl_(std::make_unique<access_device::AccessDeviceEngine>())
	, reserved_devices_(reserved_devices)
{
}

void AccessDevicesReservedEngine::remove(const DeviceId& device_name)  {
	if (reserved_devices_ == nullptr
		|| !reserved_devices_->contains(device_name, CardReader))
		return impl_->remove(device_name);
}
