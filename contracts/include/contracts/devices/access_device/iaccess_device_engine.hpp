#ifndef IAccessDeviceEngine_Included
#define IAccessDeviceEngine_Included

#include <contracts/devices/idevice_engine.hpp>
#include <contracts/devices/access_device/icommand_result.hpp>

namespace contracts
{
	namespace devices
	{
		namespace access_device
		{
			class IAccessDeviceEngine : public locations::IDeviceEngine<ICommandResult>
			{
			public:
				virtual ~IAccessDeviceEngine() {}

				virtual void execute( const data_model::DeviceId& device_name
					                  , lights data = lNone) = 0;
			};

			typedef std::shared_ptr<IAccessDeviceEngine> IAccessDeviceEnginePtr;

		}

	}
}

#endif
