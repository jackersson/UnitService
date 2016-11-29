#ifndef DirectShowDeviceEnumerator_Included
#define DirectShowDeviceEnumerator_Included

#include <devices/idevice_enumerator.hpp>
#include <devices/idevice_info.hpp>
#include <mutex>
#include "directshow_device_info.hpp"
#include <threadable.hpp>

namespace video_device
{
	class Capability;
	class DirectShowDeviceInfo;

	class DirectshowDeviceEnumerator : public utils::Threadable
		, public devices::IDeviceEnumerator
		, public devices::IDeviceInfo<DirectShowDeviceInfo>
	{
	public:
		DirectshowDeviceEnumerator();
		~DirectshowDeviceEnumerator();
		bool connected(const data_model::DeviceId& device_name) const override;
		void enumerate(std::vector<data_model::DeviceId>&) const override;

		
		DirectShowDeviceInfo 
			get_device(const data_model::DeviceId& device_name) const override;

		bool try_get_device( const data_model::DeviceId& device_name
			                 , DirectShowDeviceInfo&) const override;


	
	protected:
		void run() override;

	private:
		void print    ();
		void enumerate();
		void update   ();

		bool connected(const std::string& device_name) const;

		bool do_connected(const std::string& device_name) const;


		DirectshowDeviceEnumerator(const DirectshowDeviceEnumerator& other) = delete;
		DirectshowDeviceEnumerator& operator=(const DirectshowDeviceEnumerator&) = delete;
		
		mutable std::mutex mutex_;

		std::vector<std::string>          devices_;
		std::vector<DirectShowDeviceInfo> actual_ ;
	};
}

#endif
