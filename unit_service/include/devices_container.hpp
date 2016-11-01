#ifndef DevicesContainer_Included
#define DevicesContainer_Included

#include <contracts/devices/idevices_container.hpp>
#include <access_device/access_device_engine.hpp>
#include <directshow_device_engine.hpp>
#include "access_devices_reserved_engine.hpp"
#include "directshow_reserved_engine.hpp"

class DevicesContainer : public contracts::devices::IDevicesContainer	                    
{
public:
	DevicesContainer() 
		: access_engine_    (std::make_unique<access_device::AccessDeviceEngine>())
		, directshow_engine_(std::make_unique<directshow_device::DirectShowDeviceEngine>())
	{	}

	explicit 
		DevicesContainer(IDevicesSet* reserved_devices)
		: access_engine_    (std::make_unique<AccessDevicesReservedEngine>(reserved_devices))
		, directshow_engine_(std::make_unique<DirectShowReservedEngine>(reserved_devices))
	{	}

	void init() override
	{
		access_engine_->init();
		directshow_engine_->init();
	}

	void de_init() override
	{
		access_engine_->de_init();
		directshow_engine_->init();
	}

	contracts::devices::access_device::IAccessDeviceEngine*
		access_device_engine() override {
		return access_engine_.get();
	}

	contracts::devices::video_device::IVideoEngine*
		directshow_device_engine() override
	{
		return directshow_engine_.get();
	}
		
	void enumerate(DataTypes::Devices& devices) const override
	{
		fill_access_devices(devices);
		fill_video_devices (devices);		
	}

	bool contains( const std::string& device_name
		           , DataTypes::DeviceType dev_type) const override
	{	
		throw std::exception("Device container contains - not implemented");
	}

private:
	void fill_access_devices(DataTypes::Devices& devices) const
	{	
		auto items = access_engine_->device_enumerator().devices();
		fill_devices(devices, items, DataTypes::DeviceType::CardReader);	
	}

	void fill_video_devices(DataTypes::Devices& devices) const
	{
		auto items = directshow_engine_->device_enumerator().devices();
		fill_devices(devices, items, DataTypes::DeviceType::Capture);	
	}

	static void fill_devices( DataTypes::Devices& devices
		                      , const std::vector<std::string>& items
		                      , DataTypes::DeviceType device_type)
	{
		for (auto item : items)
		{
			auto dev = devices.add_devices();
			dev->set_device_name(item);
			dev->set_device_type(device_type);
		}
	}
	
	std::unique_ptr<contracts::devices::access_device::IAccessDeviceEngine>
		                                              access_engine_;

	std::unique_ptr<contracts::devices::video_device::IVideoEngine>
		                                          directshow_engine_;

};

#endif