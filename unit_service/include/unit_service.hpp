#ifndef UnitService_Included
#define UnitService_Included

#include <contracts/common/ilifecycle.hpp>
#include <contracts/iunit_context.hpp>
#include <track_location_coordinator.hpp>
#include <services_coordinator.hpp>
#include <repository_container.hpp>
#include <contracts/locations/itrack_location_coordinator.hpp>
#include "devices_container.hpp"

class UnitService : public contracts::common::IModule
	, public contracts::IUnitContext
	, public std::enable_shared_from_this<contracts::IUnitContext>
{
public:
	explicit UnitService( std::shared_ptr<contracts::IUnitConfiguration> configuration)
		: configuration_(configuration)
		, logger_(std::make_shared<contracts::common::Logger>())
	{}

	~UnitService(){
		UnitService::de_init();
	}

	void init() override
	{
		logger()->info("Unit service start init");

		auto this_ptr = shared_from_this();

		devices_ = std::make_shared<DevicesContainer>();
		devices_->init();
		
		services_ = std::make_shared<grpc_services::ServicesCoordinator>(this_ptr);
		services_->init();
		/*
		repository_ = std::make_shared<data_core::RepositoryContainer>(this_ptr);
		repository_->init();

		tracking_coordinator_
			= std::make_shared<tracking::locations::TrackLocationCoordinator>(this_ptr);
			*/
		logger()->info("Unit service init done");
	}

	void de_init() override
	{
		if (services_ != nullptr)
	  	services_->de_init();

		if (devices_ != nullptr)
		  devices_->de_init();

		if (repository_ != nullptr)
		  repository_->de_init();
	}

	std::shared_ptr<contracts::devices::IDevicesContainer> devices() override	{
		return devices_;
	}

	std::shared_ptr<contracts::data::IRepositoryContainer> repository() override {
		return repository_;
	}

	std::shared_ptr<contracts::services::IServices>        services() override {
		return services_;
	}

	std::shared_ptr<contracts::common::Logger>             logger() override{
		return logger_;
	}

	std::shared_ptr<contracts::IUnitConfiguration>       configuration() override	{
		return configuration_;
	}

	std::shared_ptr<contracts::locations::ITrackLocationsCoordinator>
		track_locations() override
	{
		return tracking_coordinator_;
	}

	std::shared_ptr<contracts::IUnitConfiguration> configuration_;

	contracts::locations::ITrackLocationsCoordinatorPtr tracking_coordinator_;
	std::shared_ptr<contracts::services::IServices>        services_;
	std::shared_ptr<contracts::common::Logger>             logger_;
	std::shared_ptr<contracts::devices::IDevicesContainer> devices_;
	std::shared_ptr<contracts::data::IRepositoryContainer> repository_; //database service
};

#endif
