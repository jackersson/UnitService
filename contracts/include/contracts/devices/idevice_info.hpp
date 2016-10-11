#ifndef IDeviceInfo_Included
#define IDeviceInfo_Included

#include <string>

namespace contracts
{
	namespace devices
	{
		template <typename T>
		class IDeviceInfo 
		{
		public:	
			virtual ~IDeviceInfo() {}

			virtual bool try_get_info(const std::string& device_name, T& info) = 0;
		};
	}
}

#endif

