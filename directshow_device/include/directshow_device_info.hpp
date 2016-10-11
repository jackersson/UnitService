#ifndef DirectShowDeviceInfo_Included
#define DirectShowDeviceInfo_Included

#include <string>

namespace directshow_device
{
	class DirectShowDeviceInfo 
	{	

	public:
		DirectShowDeviceInfo(const std::string& device_name, int device_id)
		                  	: device_name_(device_name)
			                  , device_id_(device_id)
		{}

		int id() const	{
			return device_id_;
		}
		
		const std::string& name() const	{
			return device_name_;
		}

	private:
		std::string device_name_;
		int device_id_;
	
	};
}

#endif
