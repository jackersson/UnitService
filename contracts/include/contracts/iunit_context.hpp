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

		virtual const std::string& facial_service_address() const = 0;
		virtual const std::string& coordinator_service_address() const = 0;

		virtual uint16_t    unit_service_port() const = 0;

		virtual const std::string& service_uuid() const = 0;

		virtual const std::string& database_service_address() const = 0;
	};

	class IUnitContext
	{
	public:
		virtual ~IUnitContext() {}

		virtual locations::ITrackLocationsEngine*  track_locations() = 0;
		virtual devices::IDevicesContainer*        devices        () = 0;
		virtual data::AbstractRepositoryContainer* repository     () = 0;
		virtual services::IServices*               services       () = 0;
		virtual common::LoggerPtr                  logger         () = 0;
		virtual const IUnitConfiguration&          configuration  () = 0;
	};

	typedef std::shared_ptr<IUnitContext> IUnitContextPtr;

	
}

#endif
