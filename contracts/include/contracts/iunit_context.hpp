#ifndef IUnitContainer_Included
#define IUnitContainer_Included

#include "data/irepository.hpp"
#include <contracts/devices/idevices_container.hpp>
#include <contracts/services/iserver_manager.hpp>
#include <contracts/common/logger.hpp>
#include <contracts/locations/itrack_location_coordinator.hpp>

namespace contracts
{
	class IUnitConfiguration
	{
	public:
		virtual ~IUnitConfiguration() {}

		virtual const std::string& facial_service_address() = 0;
		virtual const std::string& coordinator_service_address() = 0;

		virtual uint16_t    unit_service_port() = 0;
	};

	class IUnitContext
	{
	public:
		virtual ~IUnitContext() {}

		virtual std::shared_ptr<locations::ITrackLocationsCoordinator>
			                                                  track_locations() = 0;

		virtual std::shared_ptr<devices::IDevicesContainer> devices        () = 0;
		virtual std::shared_ptr<data::IRepositoryContainer> repository     () = 0;
		virtual std::shared_ptr<services::IServices>        services       () = 0;
		virtual std::shared_ptr<common::Logger>             logger         () = 0;
		virtual std::shared_ptr<IUnitConfiguration>         configuration  () = 0;
	};

	typedef std::shared_ptr<IUnitContext> IUnitContextPtr;

	
}

#endif
