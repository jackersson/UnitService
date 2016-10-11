#ifndef IUnitContainer_Included
#define IUnitContainer_Included

#include "data/irepository.hpp"
#include <contracts/devices/idevices_container.hpp>
#include <contracts/services/iserver_manager.hpp>

namespace contracts
{
	class IUnitContext
	{
	public:
		virtual ~IUnitContext() {}

		virtual std::shared_ptr<devices::IDevicesContainer> devices()    = 0;
		virtual std::shared_ptr<data::IRepositoryContainer> repository() = 0;
		virtual std::shared_ptr<services::IServices>        services()   = 0;
	};

	typedef std::shared_ptr<IUnitContext> IUnitContextPtr;
}

#endif
