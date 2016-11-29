#include "directshow_reserved_engine.hpp"

#include <directshow_device_engine.hpp>
#include <data/models/devices.hpp>

using namespace data_model;

DirectShowReservedEngine::DirectShowReservedEngine(devices::IDevicesSet* reserved_devices)
		: impl_(std::make_unique<video_device::DirectShowDeviceEngine>())
		, reserved_devices_(reserved_devices)
{
}

void DirectShowReservedEngine::remove(const DeviceId& device_name)  {
	if (reserved_devices_ == nullptr
		|| !reserved_devices_->contains(device_name, Capture))
	impl_->remove(device_name);
}

