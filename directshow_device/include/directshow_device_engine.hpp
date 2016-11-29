#ifndef DirectShowDeviceEngine_Included
#define DirectShowDeviceEngine_Included

#include <devices/video_device/ivideo_engine.hpp>
#include <concurrent_containers.hpp>

#include "directshow_device_enumerator.hpp"
#include "directshow_device_listener.hpp"

namespace video_device
{
	class DirectShowDeviceEngine : public IVideoEngine
	{
	public:
		DirectShowDeviceEngine();
		~DirectShowDeviceEngine();

		void stop_all() override;

		void add      (const data_model::DeviceId& device_name) override;
		void remove   (const data_model::DeviceId& device_name) override;
		bool is_active(const data_model::DeviceId& device_name) override;

		void subscribe
		(devices::IDeviceObserver<IStreamDataPtr>* observer
			, const data_model::DeviceId& device_name) override;

		void unsubscribe
		(devices::IDeviceObserver<IStreamDataPtr>* observer) override;

		bool has_observer
		( devices::IDeviceObserver<IStreamDataPtr>* observer
			, const data_model::DeviceId& device_name) override;

		void unsubscribe_all() override;

		void de_init() override;
		void init   () override;

		const devices::IDeviceEnumerator& enumerator() const override;

		size_t size() const override;			

	private:
		DirectShowDeviceEngine(const DirectShowDeviceEngine& other) = delete;
		DirectShowDeviceEngine& operator=(const DirectShowDeviceEngine&) = delete;

		bool contains_key(const data_model::DeviceId& key);


		DirectshowDeviceEnumerator device_enumerator_;
		concurrent::containers::ConcurrentMap< std::string
		        	                           , DirectshowDeviceListenerPtr> devices_;

	};
}

#endif
