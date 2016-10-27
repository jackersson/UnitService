#ifndef CoordinatorService_Included
#define CoordinatorService_Included

#include <contracts/iunit_context.hpp>
#include "coordinator_connector.hpp"
#include "coordinator_heartbeat.hpp"

class CoordinatorService : public contracts::common::IModule 
{
public:
	explicit CoordinatorService( contracts::IUnitContext* context)
		                         : context_(context)
	{
		connector_ = std::make_unique<CoordinatorConnector>(context);
		heartbeat_ = std::make_unique<CoordinatorHeartbeat>(context, connector_.get());
	}

	~CoordinatorService()
	{
		CoordinatorService::de_init();
	}

	void init() override
	{
		connector_->init();
		heartbeat_->init();
	}

	void de_init() override
	{
		connector_->de_init();
		heartbeat_->de_init();
	}

private:
	contracts::IUnitContext* context_;

	std::unique_ptr<CoordinatorConnector> connector_;
	std::unique_ptr<CoordinatorHeartbeat> heartbeat_;
};

#endif