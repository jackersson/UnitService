#ifndef DevicesContainer_Included
#define DevicesContainer_Included

#include <contracts/devices/idevices_container.hpp>
#include <access_device/access_device_engine.hpp>
#include <directshow_device_engine.hpp>



class AccessDevicesReservedEngine 
	: public contracts::devices::access_device::IAccessDeviceEngine
{
public:
	explicit
	AccessDevicesReservedEngine(contracts::devices::IDevicesSet* reserved_devices)
		       : impl_(std::make_unique<access_device::AccessDeviceEngine>())
		       , reserved_devices_(reserved_devices)
	{
		
	}

	bool grant_access(const std::string& device_name) override{
		return impl_->grant_access(device_name);
	}

	bool check(const std::string& device_name) override {
		return impl_->check(device_name);
	}

	std::string get_card(const std::string& device_name) override {
		return impl_->get_card(device_name);
	}

	void stop_all() override {
		return impl_->stop_all();
	}

	void add(const std::string& device_name) override{
		return impl_->add(device_name);
	}

	void remove(const std::string& device_name) override {
		if ( reserved_devices_ == nullptr
			|| !reserved_devices_->contains( device_name
				                            , DataTypes::DeviceType::CardReader))
	  	return impl_->remove(device_name);
	}

	bool is_active(const std::string& device_name) override	{
		return impl_->is_active(device_name);
	}

	void execute(const std::string& device_name
		, contracts::devices::access_device::lights data
		= contracts::devices::access_device::lNone) override{
		impl_->execute(device_name, data);
	}

	void subscribe( access_device::IAccessDeviceObserver* observer
		            , const std::string& device_name) override {
		impl_->subscribe(observer, device_name);
	}

	void unsubscribe(access_device::IAccessDeviceObserver* observer) override	{
		impl_->unsubscribe(observer);
	}

	bool has_observer( access_device::IAccessDeviceObserver* observer
		               , const std::string& device_name) override	{
		return impl_->has_observer(observer, device_name);
	}

	void unsubscribe_all() override {
		return impl_->unsubscribe_all();
	}

	const contracts::devices::IDeviceEnumerator& device_enumerator() const override	{
		return impl_->device_enumerator();
	}

	void de_init() override	{
		return impl_->de_init();
	}

	void init() override{
		impl_->init();
	}

private:
	std::unique_ptr<IAccessDeviceEngine>  impl_;
	contracts::devices::IDevicesSet*     reserved_devices_;
};


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
		access_device_engine() override
	{
		return access_device_engine_.get();
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