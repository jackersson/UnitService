#ifndef UnitService_Included
#define UnitService_Included

#include <contracts/iunit_context.hpp>
#include <services_coordinator.hpp>
#include <contracts/locations/itrack_location_coordinator.hpp>
#include "coordinator_service.hpp"
#include "track_locations_udapter.hpp"
#include <common/ilifecycle.hpp>
#include <common/logger.hpp>

class UnitService : public contracts::common::IModule
	                , public contracts::IUnitContext
{
public:
	UnitService() : configuration_(nullptr)		            
	{}
	
	~UnitService(){
		UnitService::de_init();
	}

	void set_configuration(contracts::IUnitConfiguration* configuration) {
		configuration_ = configuration;
	}
	
	void init()    override;
	void de_init() override;

	contracts::devices::IDevicesContainer* devices() override	{
		return devices_.get();
	}

	contracts::data::AbstractRepositoryContainer* repository() override {
		return repository_.get();
	}

	contracts::services::IServices*  services() override {
		return services_.get();
	}
	
	const contracts::IUnitConfiguration&       configuration() override	{
		return *configuration_;
	}

	contracts::locations::ITrackLocationsEngine* track_locations() override
	{
		return tracking_coordinator_.get();
	}

private:
	contracts::IUnitConfiguration* configuration_;

	contracts::locations::ITrackLocationsEnginePtr                tracking_coordinator_;
	std::unique_ptr<contracts::services::IServices>               services_  ;
	std::unique_ptr<contracts::devices::IDevicesContainer>        devices_   ;
	std::unique_ptr<contracts::data::AbstractRepositoryContainer> repository_; 

	std::unique_ptr<TrackLocationsUpdater>	track_locations_updator_;

	std::unique_ptr<CoordinatorService> coordinator_service_;

	std::list<IModule*> modules_;

	contracts::logging::Logger logger_;
};

#endif
