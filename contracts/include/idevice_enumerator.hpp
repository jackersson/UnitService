#ifndef IDeviceEnumerator_Included
#define IDeviceEnumerator_Included
#include <string>
#include <vector>


namespace contracts {
	namespace devices
	{
		class idevice_enumerator
		{
		public:
			virtual ~idevice_enumerator() {}

			virtual	bool connected(const std::string& device_name) const = 0;

			virtual std::vector<std::string> devices() const = 0;
		};
	}
}

#endif
