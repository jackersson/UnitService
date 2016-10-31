#ifndef DevicesContainer_Included
#define DevicesContainer_Included

#include <contracts/devices/idevices_container.hpp>
#include <access_device/access_device_engine.hpp>
#include <directshow_device_engine.hpp>
#include "access_devices_reserved_engine.hpp"


class DevicesContainer : public contracts::devices::IDevicesContainer	                    
{
public:
	DevicesContainer() 
		: access_device_engine_ (std::make_unique<access_device::AccessDeviceEngine>())
		, direct_show_device_engine_(std::make_unique<directshow_device::DirectShowDeviceEngine>())
	{	}

	explicit 
		DevicesContainer(IDevicesSet* reserved_devices)
		: access_device_engine_(std::make_unique<AccessDevicesReservedEngine>(reserved_devices))
		, direct_show_device_engine_(std::make_unique<directshow_device::DirectShowDeviceEngine>())

	{	}

	void init() override
	{
		access_device_engine_->init();
		direct_show_device_engine_->init();
	}

	void de_init() override
	{
		access_device_engine_->de_init();
		direct_show_device_engine_->init();
	}

	contracts::devices::access_device::IAccessDeviceEngine*
		access_device_engine() override {
		return access_device_engine_.get();
	}

	contracts::devices::video_device::IVideoEngine*
		directshow_device_engine() override
	{
		return direct_show_device_engine_.get();
	}
		
	void enumerate(DataTypes::Devices& devices) const override
	{
		fill_access_devices(devices);
		fill_video_devices (devices);		
	}

private:
	void fill_access_devices(DataTypes::Devices& devices) const
	{	
		auto items = access_device_engine_->device_enumerator().devices();
		fill_devices(devices, items, DataTypes::DeviceType::CardReader);	
	}

	void fill_video_devices(DataTypes::Devices& devices) const
	{
		auto items = direct_show_device_engine_->device_enumerator().devices();
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
		                                                      access_device_engine_;


	//TODO make interface
	std::unique_ptr<directshow_device::DirectShowDeviceEngine>
		                                          direct_show_device_engine_;

};

#endif