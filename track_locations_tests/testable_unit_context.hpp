#ifndef TestableUnitService_Included
#define TestableUnitService_Included

#include <contracts/common/ilifecycle.hpp>
#include <contracts/iunit_context.hpp>
#include <contracts/locations/itrack_location_coordinator.hpp>
#include <list>
#include <devices_container.hpp>
#include <services_coordinator.hpp>
#include <repository_container.hpp>
#include <track_locations_udapter.hpp>
#include <track_locations_engine.hpp>
#include <coordinator_service.hpp>

class UnitService : public contracts::common::IModule
	                , public contracts::IUnitContext
{
public:
	UnitService() : configuration_(nullptr)
		            , logger_(std::make_unique<contracts::common::Logger>())
	{}

	~UnitService() {
		UnitService::de_init();
	}

	void set_configuration(contracts::IUnitConfiguration* configuration) {
		configuration_ = configuration;
	}

	void init()    override
	{
		logger()->info("Unit service start init");

		devices_ = std::make_unique<DevicesContainer>();
		devices_->init();

		services_ = std::make_unique<grpc_services::ServicesCoordinator>(this);
		//modules_.push_back(services_.get());

		repository_ = std::make_unique<data_core::RepositoryContainer>(this);
		modules_.push_back(repository_.get());

		tracking_coordinator_
			= std::make_unique<tracking::locations::TrackLocationsEngine>(this);
		modules_.push_back(tracking_coordinator_.get());

		track_locations_updator_ = std::make_unique<TrackLocationsUpdater>(this);
		//modules_.push_back(track_locations_updator_.get());

		coordinator_service_ = std::make_unique<CoordinatorService>(this);
		//modules_.push_back(coordinator_service_.get());

		for (auto module : modules_)
		{
			if (module != nullptr)
				module->init();
		}

		logger()->info("Unit service init done");
	}

	void de_init() override
	{
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

	contracts::common::LoggerPtr  logger() override {
		return logger_;
	}

	const contracts::IUnitConfiguration& configuration() override {
		return *configuration_;
	}

	contracts::locations::ITrackLocationsEngine* track_locations() override {
		return tracking_coordinator_.get();
	}

private:
	contracts::IUnitConfiguration* configuration_;

	std::unique_ptr<contracts::locations::ITrackLocationsEngine>  tracking_coordinator_;
	std::unique_ptr<contracts::services::IServices>               services_;
	std::shared_ptr<contracts::common::Logger>                    logger_;
	std::unique_ptr<contracts::devices::IDevicesContainer>        devices_;
	std::unique_ptr<contracts::data::AbstractRepositoryContainer> repository_;

	std::unique_ptr<TrackLocationsUpdater>	track_locations_updator_;

	std::unique_ptr<CoordinatorService> coordinator_service_;

	std::list<IModule*> modules_;
};

#endif
