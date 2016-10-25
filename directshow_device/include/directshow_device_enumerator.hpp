#ifndef DirectShowDeviceEnumerator_Included
#define DirectShowDeviceEnumerator_Included

#include <contracts/devices/idevice_enumerator.hpp>
#include <contracts/devices/idevice_info.hpp>
#include "directshow_device_info.hpp"
#include "std_threadable.hpp"
#include <mutex>

namespace directshow_device
{
	class DirectshowDeviceEnumerator : public utils::StdThreadable,
		  public contracts::devices::IDeviceEnumerator
		, public contracts::devices::IDeviceInfo<DirectShowDeviceInfo>
	{
	public:
		DirectshowDeviceEnumerator() {}
		bool connected(const std::string& device_name) const override;

		std::vector<std::string> devices() const override;
		bool try_get_info(const std::string& device_name, DirectShowDeviceInfo& info) override;
	
	protected:
		void run() override;

	private:
		void print    ();
		void enumerate();
		void update   ();

		DirectshowDeviceEnumerator(const DirectshowDeviceEnumerator& other) = delete;
		DirectshowDeviceEnumerator& operator=(const DirectshowDeviceEnumerator&) = delete;
		
		std::recursive_mutex mutex_;

		std::vector<std::string> devices_;
		std::vector<DirectShowDeviceInfo> actual_ ;
	};
}

#endif
