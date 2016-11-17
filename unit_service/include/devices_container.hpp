#ifndef DevicesContainer_Included
#define DevicesContainer_Included

#include <contracts/devices/idevices_container.hpp>

class DevicesContainer : public contracts::devices::IDevicesContainer	                    
{
public:
	DevicesContainer();

	explicit DevicesContainer(IDevicesSet* reserved_devices);

	~DevicesContainer();

	void init() override;

	void de_init() override;

	contracts::devices::access_device::IAccessDeviceEngine*
		access_device_engine() override;

	contracts::devices::video_device::IVideoEngine*
		directshow_device_engine() override;
		
	void enumerate(data_model::Devices& devices) const override;

	bool contains( const data_model::DeviceId& device_name
		           , data_model::DeviceType dev_type        ) const override;

private:
	void fill_access_devices(data_model::Devices& devices) const;
	void fill_video_devices (data_model::Devices& devices) const;

	static void fill_devices( data_model::Devices& devices
		                      , const std::vector<data_model::DeviceId>& items
		                      , data_model::DeviceType device_type);
	
	std::unique_ptr<contracts::devices::access_device::IAccessDeviceEngine>
		                                              access_engine_;

	std::unique_ptr<contracts::devices::video_device::IVideoEngine>
		                                          directshow_engine_;

};

#endif