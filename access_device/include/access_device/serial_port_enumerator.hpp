#ifndef SerialPortEnumerator_Included
#define SerialPortEnumerator_Included

#include <threadable.hpp>
//#include <thread>
#include <contracts/devices/idevice_enumerator.hpp>
#include <chrono>

namespace access_device
{
	class SerialPortEnumerator : public utils::Threadable
		                         , public contracts::devices::IDeviceEnumerator
	{
	public:
		bool connected(const data_model::DeviceId& device_name) const override;

		std::vector<data_model::DeviceId> devices() const override;

	protected:
		void run  () override;
		void print();
	private:		
		void update();
				
		std::vector<data_model::DeviceId> devices_;
		std::vector<data_model::DeviceId> serials_;

		static std::chrono::milliseconds delay_;
	};
}

#endif