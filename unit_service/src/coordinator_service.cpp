#include "coordinator_service.hpp"
#include <coordinator_utils.hpp>
#include <contracts/iservice_context.hpp>

using services::helpers::CoordinatorConnector;
using services::helpers::CoordinatorHeartbeat;

CoordinatorService::CoordinatorService(contracts::IServiceContext* context)
	: context_(context)
{
	auto coordinator = context->services()->coordinator();
	connector_
		= std::make_unique<CoordinatorConnector>(coordinator, *this);
	heartbeat_
		= std::make_unique<CoordinatorHeartbeat>( coordinator
			                                      , connector_.get()
			                                      , *this);
}

void CoordinatorService::init()
{
	connector_->init();
	heartbeat_->init();
}

void CoordinatorService::de_init()
{
	connector_->de_init();
	heartbeat_->de_init();
}


data_model::ConnectMsg CoordinatorService::connect_msg() const
{
	return services::helpers::generate_connect_msg(
	  	context_->configuration().unit_service_port()
		, context_->configuration().service_uuid()
		, data_model::ServiceType::UnitService);
}

data_model::HeartbeatMessage CoordinatorService::heartbeat_msg() const
{
	return services::helpers::generate_heartbeat_msg(
	  	context_->configuration().service_uuid()
	 	, data_model::ServiceType::UnitService);
}

