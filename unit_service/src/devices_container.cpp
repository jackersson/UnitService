#include "devices_container.hpp"

#include <devices/idevices_container.hpp>
#include <access_device/access_device_engine.hpp>
#include <directshow_device_engine.hpp>
#include "access_devices_reserved_engine.hpp"
#include "directshow_reserved_engine.hpp"

#include <data/models/devices.hpp>

DevicesContainer::DevicesContainer()
	: access_engine_(std::make_unique<access_device::AccessDeviceEngine>())
	, directshow_engine_(std::make_unique<video_device::DirectShowDeviceEngine>())
{
	DevicesContainer::init();
}

DevicesContainer::DevicesContainer(IDevicesSet* reserved_devices)
	: access_engine_(std::make_unique<AccessDevicesReservedEngine>(reserved_devices))
	, directshow_engine_(std::make_unique<DirectShowReservedEngine>(reserved_devices))
{
	DevicesContainer::init();
}

DevicesContainer::~DevicesContainer() {
	DevicesContainer::de_init();
}

void DevicesContainer::init()
{
	access_engine_->init();
	directshow_engine_->init();
}

void DevicesContainer::de_init()
{
	access_engine_->de_init();
	directshow_engine_->de_init();
}

access_device::IAccessDeviceEngine* DevicesContainer::access_device_engine() {
	return access_engine_.get();
}

video_device::IVideoEngine*	DevicesContainer::directshow_device_engine()
{
	return directshow_engine_.get();
}

void DevicesContainer::enumerate(data_model::Devices& devices) const 
{
	fill_access_devices(devices);
	fill_video_devices(devices);
}

bool DevicesContainer::contains( const data_model::DeviceId& device_name
	                             , data_model::DeviceType dev_type) const 
{
	throw std::exception("Device container contains - not implemented");
}

void DevicesContainer::fill_access_devices(data_model::Devices& devices) const
{
	std::vector<data_model::DeviceId> items;
	access_engine_->enumerator().enumerate(items);
	fill_devices(devices, items, data_model::DeviceType::CardReader);
}

void DevicesContainer::fill_video_devices(data_model::Devices& devices) const
{
	std::vector<data_model::DeviceId> items;
	directshow_engine_->enumerator().enumerate(items);
	fill_devices(devices, items, data_model::DeviceType::Capture);
}

void DevicesContainer::fill_devices(data_model::Devices& devices
	                       , const std::vector<data_model::DeviceId>& items
	                       , data_model::DeviceType device_type)
{
	for (auto& item : items)
		devices.add(data_model::Device(item, device_type));
}

