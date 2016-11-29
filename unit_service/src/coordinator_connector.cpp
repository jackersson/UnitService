#include "coordinator_connector.hpp"

#include <data/models/unit.hpp>
#include <data/data_utils.hpp>
#include <network_utils.hpp>
#include <services/service_address.hpp>

CoordinatorConnector::CoordinatorConnector(
	  contracts::services::ICoordinatorApi* coordinator
	, contracts::IServiceConfiguration* configuration)
	: connected_    (false)
	, coordinator_  (coordinator)
	, configuration_(configuration)
	, retries_count(0)
{
	if (coordinator_ == nullptr )
		throw std::exception("Coordinator can't be null");

	if (configuration_ == nullptr)
		throw std::exception("Configuration can't be null");
}

void CoordinatorConnector::action()
{
	if (connected_)
	{
		auto ok = coordinator_->heart_beat(heartbeat_msg());
		//logger_.info("Hearbeat {0}", ok);
		if (!ok)
		{
			retries_count++;
			if (retries_count > max_retries_count)
				connected_ = false;
		}
	}
	else	
		 try_connect();	
}

bool CoordinatorConnector::try_connect()
{
	if (coordinator_ == nullptr)
		return false;

	try
	{
		connected_ = coordinator_->connect_request(connect_msg());
		if (connected_)
		{
			retries_count = 0;
			logger_.info("Connected to coordinator successfully");
		}
	}
	catch (std::exception& exception)
	{
		logger_.info("Coordinator client can't connect. {0}", exception.what());
	}
	return connected_;
}

void CoordinatorConnector::init()
{
	if (repeatable_action_ != nullptr)
		return;

	repeatable_action_
		= std::make_unique<utils::threading::RepeatableAction>(this, DELAY);
	repeatable_action_->start();
}

void CoordinatorConnector::de_init()
{
	if (repeatable_action_ != nullptr)
		repeatable_action_->stop();
}

const data_model::ConnectMsg& CoordinatorConnector::connect_msg() const
{
	static auto msg = generate_connect_msg(
		  configuration_->unit_service_address()
		, configuration_->service_uuid()
		, data_model::ServiceType::UnitService);

	return msg;
}

const data_model::HeartbeatMessage& CoordinatorConnector::heartbeat_msg() const
{
	static auto msg = generate_heartbeat_msg(
		  configuration_->service_uuid()
		, data_model::ServiceType::UnitService);

	return msg;
}


data_model::ConnectMsg
generate_connect_msg(const std::string& unit_service_address
	, const std::string& service_id
	, data_model::ServiceType service_type)
{
	data_model::ConnectMsg connect_msg;
	connect_msg.ip_address = unit_service_address;
	connect_msg.type       = service_type;
	connect_msg.id = contracts::data::to_data_key(service_id);

	return connect_msg;
}

data_model::HeartbeatMessage
generate_heartbeat_msg( const std::string& service_id
	                    , data_model::ServiceType   service_type)
{
	data_model::HeartbeatMessage message;
	message.type = service_type;
	message.id = contracts::data::to_data_key(service_id);

	return message;
}