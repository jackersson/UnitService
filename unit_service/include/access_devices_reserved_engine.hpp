#ifndef AccessDevicesReservedEngine_Included
#define AccessDevicesReservedEngine_Included

#include <contracts/devices/idevices_container.hpp>

class AccessDevicesReservedEngine
	: public contracts::devices::access_device::IAccessDeviceEngine
{
public:
	explicit
		AccessDevicesReservedEngine(contracts::devices::IDevicesSet* reserved_devices);

	void stop_all() override {
		return impl_->stop_all();
	}

	void add(const data_model::DeviceId& device_name) override {
		return impl_->add(device_name);
	}

	void remove(const data_model::DeviceId& device_name) override;

	bool is_active(const data_model::DeviceId& device_name) override {
		return impl_->is_active(device_name);
	}

	void execute(const data_model::DeviceId& device_name
		, contracts::devices::access_device::lights data
		= contracts::devices::access_device::lNone) override {
		impl_->execute(device_name, data);
	}

	void subscribe(contracts::devices::IDeviceObserver
		<contracts::devices::access_device::ICommandResult>* observer
		, const data_model::DeviceId& device_name) override {
		impl_->subscribe(observer, device_name);
	}

	void unsubscribe(contracts::devices::IDeviceObserver
		              <contracts::devices::access_device::ICommandResult>* observer) override {
		impl_->unsubscribe(observer);
	}

	bool has_observer(contracts::devices::IDeviceObserver
		               <contracts::devices::access_device::ICommandResult>* observer
		, const data_model::DeviceId& device_name) override {
		return impl_->has_observer(observer, device_name);
	}

	void unsubscribe_all() override {
		return impl_->unsubscribe_all();
	}

	const contracts::devices::IDeviceEnumerator& enumerator() const override {
		return impl_->enumerator();
	}

	size_t size() const override{
		return impl_->size();
	}
	
	void de_init() override {
		return impl_->de_init();
	}

	void init() override {
		impl_->init();
	}

private:
	std::unique_ptr<IAccessDeviceEngine>  impl_;
	contracts::devices::IDevicesSet*     reserved_devices_;
};

#endif