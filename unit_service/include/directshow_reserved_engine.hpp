#ifndef DirectShowReservedEngine_Included
#define DirectShowReservedEngine_Included

#include <contracts/devices/idevices_container.hpp>

class DirectShowReservedEngine
	: public contracts::devices::video_device::IVideoEngine
{
public:
	explicit
		DirectShowReservedEngine(contracts::devices::IDevicesSet* reserved_devices);

	void stop_all() override {
		return impl_->stop_all();
	}

	void add(const std::string& device_name) override {
		return impl_->add(device_name);
	}

	void remove(const std::string& device_name) override;

	bool is_active(const std::string& device_name) override {
		return impl_->is_active(device_name);
	}
	
	void subscribe(contracts::devices::IDeviceObserver
		<contracts::devices::video_device::IStreamData>* observer
		, const std::string& device_name) override {
		impl_->subscribe(observer, device_name);
	}

	void unsubscribe(contracts::devices::IDeviceObserver
		<contracts::devices::video_device::IStreamData>* observer) override {
		impl_->unsubscribe(observer);
	}

	bool has_observer(contracts::devices::IDeviceObserver
		<contracts::devices::video_device::IStreamData>* observer
		, const std::string& device_name) override {
		return impl_->has_observer(observer, device_name);
	}

	void unsubscribe_all() override {
		return impl_->unsubscribe_all();
	}

	const contracts::devices::IDeviceEnumerator& device_enumerator() const override {
		return impl_->device_enumerator();
	}

	void de_init() override {
		return impl_->de_init();
	}

	void init() override {
		impl_->init();
	}

private:
	std::unique_ptr<IVideoEngine>    impl_            ;
	contracts::devices::IDevicesSet* reserved_devices_;
};

#endif