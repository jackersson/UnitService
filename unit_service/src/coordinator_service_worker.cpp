#include "coordinator_service_worker.hpp"

#include <coordinator_utils.hpp>
#include <coordinator_service_connector.hpp>
#include <coordinator_service_heartbeat.hpp>
#include <data/models/unit.hpp>

using namespace services::helpers;

CoordinatorServiceWorker::CoordinatorServiceWorker(
	                           contracts::IServiceContext* context)
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

CoordinatorServiceWorker::~CoordinatorServiceWorker() {
	CoordinatorServiceWorker::de_init();
}

void CoordinatorServiceWorker::init()
{
	connector_->init();
	heartbeat_->init();
}

void CoordinatorServiceWorker::de_init()
{
	connector_->de_init();
	heartbeat_->de_init();
}

const data_model::ConnectMsg& CoordinatorServiceWorker::connect_msg() const
{
	static auto msg  = generate_connect_msg(
	  	context_->configuration().unit_service_port()
		, context_->configuration().service_uuid()
		, data_model::ServiceType::UnitService);

	return msg;
}

const data_model::HeartbeatMessage& CoordinatorServiceWorker::heartbeat_msg() const
{
	static auto msg = generate_heartbeat_msg(
	                    	context_->configuration().service_uuid()
	                   	, data_model::ServiceType::UnitService);

	return msg;
}

