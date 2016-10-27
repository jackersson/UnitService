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

				virtual void execute( const std::string& device_name
					                  , lights data = lNone) = 0;

				virtual bool grant_access(const std::string& device_name) = 0;

				virtual bool check(const std::string& device_name) = 0;

				virtual std::string get_card(const std::string& device_name) = 0;
			};

			typedef std::shared_ptr<IAccessDeviceEngine> IAccessDeviceEnginePtr;

		}

	}
}

#endif
