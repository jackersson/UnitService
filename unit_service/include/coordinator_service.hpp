#ifndef CoordinatorService_Included
#define CoordinatorService_Included

#include <contracts/iservice_context.hpp>
#include "coordinator_service_connector.hpp"
#include "coordinator_service_heartbeat.hpp"

class CoordinatorService : public contracts::common::IModule 
	                       , public contracts::services::ICoordinatorMessages
	
{
public:
	explicit CoordinatorService(contracts::IServiceContext* context);

	~CoordinatorService(){
		CoordinatorService::de_init();
	}

	void init   () override;
	void de_init() override;

	data_model::ConnectMsg       connect_msg  () const override;
	data_model::HeartbeatMessage heartbeat_msg() const override;

private:
	contracts::IServiceContext* context_;

	std::unique_ptr<services::helpers::CoordinatorConnector> connector_;
	std::unique_ptr<services::helpers::CoordinatorHeartbeat> heartbeat_;
};

#endif