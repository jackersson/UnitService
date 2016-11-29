#ifndef IDeviceObserver_Included
#define IDeviceObserver_Included

#include <devices/device_exception.hpp>

namespace data_model
{
	enum DeviceState;
}

namespace devices
{		
	class IDeviceState
	{
	public:
		virtual ~IDeviceState() {}

		virtual data_model::DeviceState state() const = 0;
		virtual data_model::DeviceType  type () const = 0;
	};

	class IDeviceStateObserver
	{
	public:
		virtual ~IDeviceStateObserver() {}

		virtual void on_error(const DeviceException& exception) = 0;
		virtual void on_state(const IDeviceState&    state    ) = 0;
	};

	template <typename T>
	class IDeviceObserver : public IDeviceStateObserver
	{
	public:
		virtual ~IDeviceObserver() {}

		virtual void on_next(T data) = 0;
	};	
}

#endif
