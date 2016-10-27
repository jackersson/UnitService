#include "unit_service.hpp"
#include <repository_container.hpp>
#include <devices_container.hpp>
#include <track_locations_engine.hpp>

void UnitService::init() 
{
	logger()->info("Unit service start init");
	
	devices_ = std::make_unique<DevicesContainer>();
	modules_.push_back(devices_.get());

	services_ = std::make_unique<grpc_services::ServicesCoordinator>(this);
	modules_.push_back(services_.get());
		
	repository_ = std::make_unique<data_core::RepositoryContainer>(this);
	modules_.push_back(repository_.get());
		
	tracking_coordinator_
  	= std::make_shared<tracking::locations::TrackLocationsEngine>(this);
	modules_.push_back(tracking_coordinator_.get());

	track_locations_updator_
		= std::make_unique<TrackLocationsUpdater>(this);
	modules_.push_back(track_locations_updator_.get());

	coordinator_service_ = std::make_unique<CoordinatorService>(this);
	modules_.push_back(coordinator_service_.get());

	for (auto module : modules_)
	{
		if (module != nullptr)
			module->init();
	}

	logger()->info("Unit service init done");
}

void UnitService::de_init() 
{	
	for( auto module : modules_ )
	{
		if ( module != nullptr)
		  module->de_init();
	}
}