#ifndef IDeviceInfo_Included
#define IDeviceInfo_Included

namespace data_model
{
	class DeviceId;
}

namespace contracts
{
	namespace devices
	{
		template <typename T>
		class IDeviceInfo 
		{
		public:	
			virtual ~IDeviceInfo() {}

			virtual bool try_get_info( const data_model::DeviceId& device_name
				                       , T& info) = 0;
		};
	}
}

#endif

