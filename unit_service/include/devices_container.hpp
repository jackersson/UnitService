#ifndef DevicesContainer_Included
#define DevicesContainer_Included

#include <contracts/common/ilifecycle.hpp>
#include <contracts/devices/idevices_container.hpp>
#include <access_device/access_device_engine.hpp>


class DevicesContainer : public contracts::devices::IDevicesContainer
{
public:
	DevicesContainer() 
		: access_device_engine_ (std::make_shared<access_device::AccessDeviceEngine>())	
	{
		
	}

	void init() override
	{
		access_device_engine_->init();
	}

	void de_init() override
	{
		access_device_engine_->de_init();
	}

	contracts::devices::access_device::IAccessDeviceEnginePtr
		access_device_engine() override
	{
		return access_device_engine_;
	}

	void enumerate(DataTypes::Devices&) const override
	{
		throw std::exception("Not implemented enumerate in devices container");
	}

private:
	std::shared_ptr<access_device::AccessDeviceEngine> access_device_engine_;
};

#endif