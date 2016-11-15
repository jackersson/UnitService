#ifndef DirectShowDeviceEngine_Included
#define DirectShowDeviceEngine_Included

#include <std_threadable.hpp>
#include <contracts/devices/video_device/ivideo_engine.hpp>
#include <concurrent_containers.hpp>

#include "directshow_device_enumerator.hpp"
#include "directshow_device_listener.hpp"

namespace directshow_device
{
	class DirectShowDeviceEngine
		: public contracts::devices::video_device::IVideoEngine
	{
	public:
		DirectShowDeviceEngine();
		~DirectShowDeviceEngine();

		void stop_all() override;

		void add      (const data_model::DeviceId& device_name) override;
		void remove   (const data_model::DeviceId& device_name) override;
		bool is_active(const data_model::DeviceId& device_name) override;

		void subscribe
		(contracts::devices::IDeviceObserver
			<contracts::devices::video_device::IStreamData>* observer
			, const data_model::DeviceId& device_name) override;

		void unsubscribe
		(contracts::devices::IDeviceObserver
			<contracts::devices::video_device::IStreamData>* observer) override;

		bool has_observer
		(contracts::devices::IDeviceObserver
			<contracts::devices::video_device::IStreamData>* observer
			, const data_model::DeviceId& device_name) override;

		void unsubscribe_all() override;

		void de_init() override;
		void init   () override;

		void enumerate_devices(std::vector<data_model::DeviceId>& devs) override;


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
