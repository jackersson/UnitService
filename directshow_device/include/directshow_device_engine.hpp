#ifndef DirectShowDeviceEngine_Included
#define DirectShowDeviceEngine_Included


#include <std_threadable.hpp>
#include <directshow_device_listener.hpp>
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
		DirectShowDeviceEngine() {
			DirectShowDeviceEngine::init();
		}

		~DirectShowDeviceEngine() {
			DirectShowDeviceEngine::de_init();
		}

		void stop_all() override
		{
			for (auto it = devices_.begin(); it != devices_.end(); ++it)
				it->second->stop();

			devices_.clear();
		}

		void add(const std::string& device_name) override
		{
			if (device_name == "")
				return;

			if (devices_.contains(device_name))
				return;

			//TODO will be device number
			auto listener = std::make_shared<DirectshowDeviceListener>(device_name
			                                                        , &device_enumerator_);
			listener->start();
			devices_.insert(device_name, listener);
		}

		void remove(const std::string& device_name) override
		{
			if (device_name == "")
				return;

			try
			{
				const auto& listener = devices_.find(device_name);
				listener->stop();
				devices_.remove(device_name);
			}
			catch (std::exception&) {
				//Not implemented
			}
		}

		bool is_active(const std::string& device_name) override
		{
			if (device_name == "")
				return false;

			try
			{
				auto& listener = devices_.find(device_name);
				auto result = listener->active();
				return result;
			}
			catch (std::exception&) {
				return false;
			}
		}

		void subscribe(IVideoDeviceObserver* observer
			, const std::string& device_name) override
		{
			if (device_name == "")
				return;

			try
			{
				auto& listener = devices_.find(device_name);
				listener->subscribe(observer);				
			}
			catch (std::exception&) {
				//Not implemented
			}
		}

		void unsubscribe(IVideoDeviceObserver* observer) override
		{
			for (auto it : devices_) {
				it.second->unsubscribe(observer);
			}
		}

		bool has_observer(IVideoDeviceObserver* observer
			, const std::string& device_name) override
		{
			if (device_name == "")
				return false;
			try
			{
				auto& listener = devices_.find(device_name);
				auto result = listener->has_observer(observer);
				return result;
			}
			catch (std::exception&) {
				return false;
			}
		}

		void unsubscribe_all() override
		{
		  for (auto it : devices_) {
					it.second->unsubscribe_all();
			}
		}

		const contracts::devices::IDeviceEnumerator& device_enumerator() const override
		{			
			return device_enumerator_;
		}

		void de_init() override
		{
			stop_all();
			device_enumerator_.stop();
		}

		void init() override
		{
			device_enumerator_.start();
		}

		bool contains_key(std::string key)
		{
			try
			{
				devices_.find(key);
				return true;
			}
			catch (std::exception&) {
				return false;
			}
		}

	private:
		DirectShowDeviceEngine(const DirectShowDeviceEngine& other) = delete;
		DirectShowDeviceEngine& operator=(const DirectShowDeviceEngine&) = delete;

		DirectshowDeviceEnumerator device_enumerator_;
		concurrent::containers::ConcurrentMap<std::string
			, DirectshowDeviceListenerPtr> devices_;

	};
}

#endif
