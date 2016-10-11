#ifndef IDeviceObserver_Included
#define IDeviceObserver_Included

#include <contracts/common/iequatable.hpp>
#include <contracts/devices/device_exception.hpp>

namespace contracts
{
	namespace devices
	{		
		class IDeviceState
		{
		public:
			virtual ~IDeviceState() {}

			virtual DataTypes::DeviceState state() const = 0;
			virtual DataTypes::DeviceType  type () const = 0;
		};

		class IDeviceStateObserver
		{
		public:
			virtual ~IDeviceStateObserver() {}

			virtual void on_error(const DeviceException& exception) = 0;
			virtual void on_state(const IDeviceState&    state    ) = 0;
		};

		template <typename T>
		class IDeviceObserver : public IDeviceStateObserver, common::IObject
		{
		public:
			virtual ~IDeviceObserver() {}

			virtual void on_next( const T& data ) = 0;
		};

		
	}
}

#endif
