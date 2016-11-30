#ifndef IServiceContext_Included
#define IServiceContext_Included

#include <devices/idevices_container.hpp>
#include "iservices.hpp"
#include <locations/itrack_location_coordinator.hpp>
#include <iservice_configuration.hpp>

namespace contracts
{
	class IServiceContext
	{
	public:
		virtual ~IServiceContext() {}

		virtual locations::ITrackLocationsEngine*  track_locations() = 0;
		virtual devices::IDevicesContainer*        devices()         = 0;
		virtual data::AbstractRepositoryContainer* repository()      = 0;
		virtual ::services::IServices*             services()        = 0;
		virtual IServiceConfiguration*             configuration()   = 0;
	};

	typedef std::shared_ptr<IServiceContext> IServiceContextPtr;
}

#endif