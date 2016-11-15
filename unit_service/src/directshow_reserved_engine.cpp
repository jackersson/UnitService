#include "directshow_reserved_engine.hpp"

#include <directshow_device_engine.hpp>
#include <data/models/devices.hpp>

using namespace data_model;

DirectShowReservedEngine::DirectShowReservedEngine(contracts::devices::IDevicesSet* reserved_devices)
		: impl_(std::make_unique<directshow_device::DirectShowDeviceEngine>())
		, reserved_devices_(reserved_devices)
{
}

void DirectShowReservedEngine::remove(const DeviceId& device_name)  {
	if (reserved_devices_ == nullptr
		|| !reserved_devices_->contains(device_name, Capture))
		return impl_->remove(device_name);
}

