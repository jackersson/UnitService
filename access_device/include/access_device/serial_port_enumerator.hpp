#ifndef SerialPortEnumerator_Included
#define SerialPortEnumerator_Included

#include <threadable.hpp>
#include <devices/idevice_enumerator.hpp>
#include <chrono>
#include <observers/observable.hpp>
#include "access_device_impl.hpp"
#include <devices/idevice_info.hpp>

namespace access_device
{
	class SerialPortEnumerator : public utils::Threadable
		                         , public devices::IDeviceEnumerator
		                         , public devices::IDeviceInfo<AccessDeviceImplPtr>
	{
	public:
		SerialPortEnumerator();
		~SerialPortEnumerator();

		bool connected(const data_model::DeviceId& device_name) const override;
		void enumerate(std::vector<data_model::DeviceId>&     ) const override;
		
		AccessDeviceImplPtr
			get_device(const data_model::DeviceId& device_name) const override;

		bool try_get_device(const data_model::DeviceId& device_name
			, AccessDeviceImplPtr& info) const override
		{
			throw std::exception("not implemented");
		}


	protected:
		void run  () override;
		void print();
	private:		
		SerialPortEnumerator(const SerialPortEnumerator& other) = delete;
		SerialPortEnumerator& operator=(const SerialPortEnumerator&) = delete;

		void update ();

		bool contains   (const std::string& port_name);
		bool do_contains(const std::string& port_name);
				
		mutable std::mutex mutex_;

		std::vector<AccessDeviceImplPtr> devices_;
		std::vector<std::string> serials_;
		
		static std::chrono::milliseconds delay_;
	};
}

#endif