#ifndef DirectShowDeviceEngine_Included
#define DirectShowDeviceEngine_Included

#include <concurrent_containers.hpp>
#include <directshow_device_enumerator.hpp>
#include <directshow_device_listener.hpp>
#include <contracts/devices/video_device/istream_data.hpp>
#include <contracts/devices/video_device/ivideo_engine.hpp>

namespace directshow_device
{
	typedef contracts::devices::IDeviceObserver<contracts::devices::video_device::IStreamData>  IStreamData;
	class DirectShowDeviceEngine : public contracts::devices::video_device::IVideoEngine
	{
	public:
		DirectShowDeviceEngine() {
			DirectShowDeviceEngine::init();
		}

		~DirectShowDeviceEngine() {}

		void stop_all() override
		{
			device_enumerator_.stop();
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
			auto listener = std::make_shared<DirectshowDeviceListener>(device_name);
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
				return listener->active();
			}
			catch (std::exception&) {
				return false;
			}
		}
		

	
		void subscribe( const IStreamData& observer
			            , const std::string& device_name) override
		{
			if (device_name == "")
				return;

			try
			{
				auto& listener = devices_.find(device_name);
				return listener->subscribe(observer);
			}
			catch (std::exception&) {
				//Not implemented
			}
		}

		void unsubscribe(const IStreamData& observer) override
		{
			for (auto it : devices_) {
				it.second->unsubscribe(observer);
			}
		}

		bool has_observer(const IStreamData& observer
			, const std::string& device_name) override
		{
			if (device_name == "")
				return false;
			try
			{
				auto& listener = devices_.find(device_name);
				return listener->has_observer(observer);
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
		DirectshowDeviceEnumerator device_enumerator_;
		concurrency::containers::ConcurrentMap< std::string
			                                    , DirectshowDeviceListenerPtr> devices_;

	};
}

#endif
