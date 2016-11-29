#ifndef IAccessDeviceEngine_Included
#define IAccessDeviceEngine_Included

#include <devices/idevice_engine.hpp>
#include <devices/access_device/icommand_result.hpp>

namespace access_device
{
	class IAccessDeviceEngine : public locations::IDeviceEngine<ICommandResultPtr>
	{
	public:
		virtual ~IAccessDeviceEngine() {}

		virtual void execute( const data_model::DeviceId& device_name
			                  , ::access_device::lights data = lNone) = 0;
	};

	typedef std::shared_ptr<IAccessDeviceEngine> IAccessDeviceEnginePtr;

}

#endif
