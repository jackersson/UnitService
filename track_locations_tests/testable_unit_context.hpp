#ifndef TestableUnitService_Included
#define TestableUnitService_Included

#include <common/ilifecycle.hpp>
#include <contracts/iservice_context.hpp>
#include <contracts/locations/itrack_location_coordinator.hpp>
#include <list>
#include <devices_container.hpp>
//#include <services_coordinator.hpp>
//#include <repository_container.hpp>
#include <track_locations_engine.hpp>
#include "testable_repository_container.hpp"

//#include <coordinator_service.hpp>

class TestableUnitContext : public contracts::common::IModule
	                        , public contracts::IServiceContext
{
public:
	TestableUnitContext() : configuration_(nullptr)
	{}

	~TestableUnitContext() {
		TestableUnitContext::de_init();
	}

	void set_configuration(contracts::IServiceConfiguration* configuration) {
		configuration_ = configuration;
	}

	void init()    override
	{
		logger_.info("Unit service start init");

		devices_ = std::make_unique<DevicesContainer>();
		devices_->init();

		//services_ = std::make_unique<grpc_services::ServicesCoordinator>(this);
		//modules_.push_back(services_.get());

		repository_ = std::make_unique<track_locations_tests::TestableRepositoryContainer>();
		modules_.push_back(repository_.get());

		tracking_coordinator_
			= std::make_unique<tracking::locations::TrackLocationsEngine>(this);
		modules_.push_back(tracking_coordinator_.get());
		
		for (auto module : modules_)
		{
			if (module != nullptr)
				module->init();
		}

		logger_.info("Unit service init done");
	}

	void de_init() override
	{
		for (auto module : modules_)
		{
			if (module != nullptr)
				module->de_init();
		}
		modules_.clear();
	}

	contracts::devices::IDevicesContainer* devices() override {
		return devices_.get();
	}

	contracts::data::AbstractRepositoryContainer* repository() override {
		return repository_.get();
	}

	contracts::services::IServices* services() override {
		return services_.get();
	}
	
	const contracts::IServiceConfiguration& configuration() override {
		return *configuration_;
	}

	contracts::locations::ITrackLocationsEngine* track_locations() override {
		return tracking_coordinator_.get();
	}

private:
	contracts::IServiceConfiguration* configuration_;

	std::unique_ptr<contracts::locations::ITrackLocationsEngine>  tracking_coordinator_;
	std::unique_ptr<contracts::services::IServices>               services_;
	std::unique_ptr<contracts::devices::IDevicesContainer>        devices_;
	std::unique_ptr<contracts::data::AbstractRepositoryContainer> repository_;
	
	contracts::logging::Logger   logger_;

	//std::unique_ptr<CoordinatorService> coordinator_service_;

	std::list<IModule*> modules_;
};

#endif
