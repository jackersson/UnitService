#ifndef UnitService_Included
#define UnitService_Included

#include <contracts/common/ilifecycle.hpp>
#include <contracts/iunit_context.hpp>
#include <services_coordinator.hpp>
#include <contracts/locations/itrack_location_coordinator.hpp>

class UnitService : public contracts::common::IModule
	                , public contracts::IUnitContext
{
public:
	UnitService() : configuration_(nullptr)
		            , logger_(std::make_shared<contracts::common::Logger>())
	{}
	
	~UnitService(){
		UnitService::de_init();
	}

	void set_configuration(contracts::IUnitConfiguration* configuration) {
		configuration_ = configuration;
	}
	
	void init()    override;
	void de_init() override;

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

	const contracts::IUnitConfiguration&       configuration() override	{
		return *configuration_;
	}

	std::shared_ptr<contracts::locations::ITrackLocationsCoordinator>
		track_locations() override
	{
		return tracking_coordinator_;
	}

private:
	void connect();

	contracts::IUnitConfiguration* configuration_;

	contracts::locations::ITrackLocationsCoordinatorPtr tracking_coordinator_;
	std::shared_ptr<contracts::services::IServices>        services_;
	std::shared_ptr<contracts::common::Logger>             logger_;
	std::shared_ptr<contracts::devices::IDevicesContainer> devices_;
	std::shared_ptr<contracts::data::IRepositoryContainer> repository_; //database service

	std::list<IModule*> modules_;
};

#endif
