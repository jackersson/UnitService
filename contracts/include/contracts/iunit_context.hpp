#ifndef IUnitContainer_Included
#define IUnitContainer_Included

#include <contracts/devices/access_device/iaccess_device_engine.hpp>
#include "data/irepository.hpp"
#include <contracts/devices/idevices_container.hpp>

namespace contracts
{
	class IUnitContext
	{
	public:
		virtual ~IUnitContext() {}

		virtual std::shared_ptr<devices::IDevicesContainer>    devices() = 0;
		virtual std::shared_ptr<data::IRepositoryContainer> repository() = 0;
	};

	typedef std::shared_ptr<IUnitContext> IUnitContextPtr;
}

#endif
