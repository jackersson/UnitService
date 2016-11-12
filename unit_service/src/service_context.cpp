#include "service_context.hpp"

#include <repository_container.hpp>
#include <devices_container.hpp>
#include <track_locations_engine.hpp>
#include <services_coordinator.hpp>
#include "coordinator_service_worker.hpp"

void ServiceContext::init()
{
	logger_.info("Unit service start init");
	
	devices_ = std::make_unique<DevicesContainer>();
	modules_.push_back(devices_.get());

	services_ = std::make_unique<grpc_services::ServicesCoordinator>(this);
	modules_.push_back(services_.get());
		
	auto database_api = services_->database();
	repository_ = std::make_unique<data_core::RepositoryContainer>(database_api);
	modules_.push_back(repository_.get());
		
	tracking_coordinator_
  	= std::make_shared<tracking::locations::TrackLocationsEngine>(this);
	modules_.push_back(tracking_coordinator_.get());	

	coordinator_service_ = std::make_unique<CoordinatorServiceWorker>(this);
	modules_.push_back(coordinator_service_.get());

	for (auto module : modules_)
	{
		if (module != nullptr)
			module->init();
	}

	logger_.info("Unit service init done");
}

void ServiceContext::de_init()
{	
	for( auto module : modules_ )
	{
		if ( module != nullptr)
		  module->de_init();
	}
}