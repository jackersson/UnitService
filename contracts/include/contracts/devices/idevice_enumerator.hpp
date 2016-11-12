#ifndef IDeviceEnumerator_Included
#define IDeviceEnumerator_Included

#include <vector>

namespace data_model{
	class DeviceId;
}

namespace contracts {
	namespace devices
	{
		class IDeviceEnumerator
		{
		public:
			virtual ~IDeviceEnumerator() {}

			virtual	bool connected(const data_model::DeviceId& device_name) const = 0;

			virtual std::vector<data_model::DeviceId> devices() const = 0;
		};
	}
}

#endif
