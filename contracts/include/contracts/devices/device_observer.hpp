#ifndef IDeviceObserver_Included
#define IDeviceObserver_Included

#include <contracts/common/iequatable.hpp>

namespace contracts
{
	namespace devices
	{
		class DeviceException;

		enum DeviceType
		{
			  NoneType
			, Card
			, Facial
			, Fingerprint
			, Iris
		};

		enum DeviceState
		{
			  NoneState
			, Active
			, StoppedByUser
			, Paused
			, Stopped
			, Error
		};

		class IDeviceState
		{
		public:
			virtual ~IDeviceState() {}

			virtual DeviceType  state() const = 0;
			virtual DeviceState type () const = 0;
		};

		class IDeviceStateObserver
		{
		public:
			virtual ~IDeviceStateObserver() {}

			virtual void on_error(const DeviceException& exception) const = 0;
			virtual void on_state(const IDeviceState&    state    ) const = 0;
		};

		template <typename T>
		class IDeviceObserver : public IDeviceStateObserver, common::IObject
		{
		public:
			virtual ~IDeviceObserver() {}

			virtual void on_next( const T& data ) const = 0;
		};

		
	}
}

#endif
