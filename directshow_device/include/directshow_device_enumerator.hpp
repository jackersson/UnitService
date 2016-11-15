#ifndef DirectShowDeviceEnumerator_Included
#define DirectShowDeviceEnumerator_Included

#include <contracts/devices/idevice_enumerator.hpp>
#include <contracts/devices/idevice_info.hpp>
#include "std_threadable.hpp"
#include <mutex>
#include "directshow_device_info.hpp"


namespace directshow_device
{
	class Capability;
	class DirectShowDeviceInfo;

	class DirectshowDeviceEnumerator : public utils::StdThreadable,
		  public contracts::devices::IDeviceEnumerator
		, public contracts::devices::IDeviceInfo<DirectShowDeviceInfo>
	{
	public:
		DirectshowDeviceEnumerator() {}

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


		DirectshowDeviceEnumerator(const DirectshowDeviceEnumerator& other) = delete;
		DirectshowDeviceEnumerator& operator=(const DirectshowDeviceEnumerator&) = delete;
		
		mutable std::recursive_mutex mutex_;

		std::vector<std::string>          devices_;
		std::vector<DirectShowDeviceInfo> actual_ ;
	};
}

#endif
