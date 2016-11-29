#ifndef ServiceContext_Included
#define ServiceContext_Included

#include <iservice_context.hpp>
#include <locations/itrack_location_coordinator.hpp>
#include <common/ilifecycle.hpp>
#include <logging/logger.hpp>
#include <list>

class CoordinatorConnector;
class CoordinatorServiceWorker;

class ServiceContext : public contracts::common::IModule
	                   , public contracts::IServiceContext                     
{
public:
	explicit ServiceContext (contracts::IServiceConfiguration* configuration);
	~ServiceContext();
		
	void init()    override;
	void de_init() override;

	devices::IDevicesContainer* devices() override	{
		return devices_.get();
	}

	contracts::data::AbstractRepositoryContainer* repository() override {
		return repository_.get();
	}

	services::IServices*  services() override {
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

	std::shared_ptr<contracts::locations::ITrackLocationsEngine>  tracking_coordinator_;
	std::unique_ptr<services::IServices>               services_  ;
	std::unique_ptr<devices::IDevicesContainer>        devices_   ;
	std::unique_ptr<contracts::data::AbstractRepositoryContainer> repository_; 
	
	std::unique_ptr<CoordinatorConnector> coordinator_service_;

//	std::unique_ptr<CoordinatorServiceWorker> worker_;

	std::list<IModule*> modules_;

	contracts::logging::Logger logger_;
};

#endif
