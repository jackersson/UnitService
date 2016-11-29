#ifndef DirectShowReservedEngine_Included
#define DirectShowReservedEngine_Included

#include <devices/idevices_container.hpp>

class DirectShowReservedEngine : public video_device::IVideoEngine
{
public:
	explicit
		DirectShowReservedEngine(devices::IDevicesSet* reserved_devices);

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
	
	void subscribe(devices::IDeviceObserver<video_device::IStreamDataPtr>* observer
		, const data_model::DeviceId& device_name) override {
		impl_->subscribe(observer, device_name);
	}

	void unsubscribe(devices::IDeviceObserver<video_device::IStreamDataPtr>* observer) override {
		impl_->unsubscribe(observer);
	}

	bool has_observer(devices::IDeviceObserver<video_device::IStreamDataPtr>* observer
		, const data_model::DeviceId& device_name) override {
		return impl_->has_observer(observer, device_name);
	}

	void unsubscribe_all() override {
		return impl_->unsubscribe_all();
	}
	
	const devices::IDeviceEnumerator& enumerator() const override {
		return impl_->enumerator();
	}

	size_t size() const override {
		return impl_->size();
	}

	void de_init() override {
		return impl_->de_init();
	}

	void init() override {
		impl_->init();
	}

private:
	std::unique_ptr<IVideoEngine>    impl_            ;
	devices::IDevicesSet* reserved_devices_;
};

#endif