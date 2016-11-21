#ifndef ServiceContext_Included
#define ServiceContext_Included

#include <contracts/iservice_context.hpp>
#include <contracts/locations/itrack_location_coordinator.hpp>
#include <common/ilifecycle.hpp>
#include <logging/logger.hpp>

class CoordinatorConnector;
class CoordinatorServiceWorker;

class ServiceContext : public contracts::common::IModule
	                   , public contracts::IServiceContext                     
{
public:
	ServiceContext (contracts::IServiceConfiguration* configuration);
	~ServiceContext();
		
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
	
	contracts::IServiceConfiguration*  configuration() override	{
		return configuration_;
	}

	contracts::locations::ITrackLocationsEngine* track_locations() override
	{
		return tracking_coordinator_.get();
	}

//	const data_model::ConnectMsg&       connect_msg  () const override;	
	//const data_model::HeartbeatMessage& heartbeat_msg() const override;


private:		
	contracts::IServiceConfiguration* configuration_;

	contracts::locations::ITrackLocationsEnginePtr                tracking_coordinator_;
	std::unique_ptr<contracts::services::IServices>               services_  ;
	std::unique_ptr<contracts::devices::IDevicesContainer>        devices_   ;
	std::unique_ptr<contracts::data::AbstractRepositoryContainer> repository_; 
	
	std::unique_ptr<CoordinatorConnector> coordinator_service_;

//	std::unique_ptr<CoordinatorServiceWorker> worker_;

	std::list<IModule*> modules_;

	contracts::logging::Logger logger_;
};

#endif
