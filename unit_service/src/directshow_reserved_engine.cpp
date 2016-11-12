#include "directshow_reserved_engine.hpp"

#include <directshow_device_engine.hpp>
#include <data/models/devices.hpp>

DirectShowReservedEngine::DirectShowReservedEngine(contracts::devices::IDevicesSet* reserved_devices)
		: impl_(std::make_unique<directshow_device::DirectShowDeviceEngine>())
		, reserved_devices_(reserved_devices)
{
}

void DirectShowReservedEngine::remove(const std::string& device_name)  {
	if (reserved_devices_ == nullptr
		|| !reserved_devices_->contains(device_name
			, data_model::DeviceType::CardReader))
		return impl_->remove(device_name);
}

