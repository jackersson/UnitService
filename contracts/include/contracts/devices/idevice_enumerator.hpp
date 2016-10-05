#ifndef IDeviceEnumerator_Included
#define IDeviceEnumerator_Included

#include <vector>

namespace contracts {
	namespace devices
	{
		class IDeviceEnumerator
		{
		public:
			virtual ~IDeviceEnumerator() {}

			virtual	bool connected(const std::string& device_name) const = 0;

			virtual std::vector<std::string> devices() const = 0;
		};
	}
}

#endif
