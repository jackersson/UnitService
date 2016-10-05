#ifndef DirectShowDeviceEnumerator_Included
#define DirectShowDeviceEnumerator_Included

#include <threadable.hpp>
#include <contracts/idevice_enumerator.hpp>

namespace directshow_device
{
	class DirectshowDeviceEnumerator : public utils::Threadable,
		 public contracts::devices::IDeviceEnumerator
	{
	public:
		bool connected(const std::string& device_name) const override;

		std::vector<std::string> devices() const override;

	protected:
		void run() override;

	private:
		void print    ();
		void enumerate();
		void update   ();

		std::vector<std::string> devices_;
		std::vector<std::string> actual_ ;
	};
}

#endif
