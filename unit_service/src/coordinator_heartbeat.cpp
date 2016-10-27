#include "coordinator_heartbeat.hpp"
#include <network_utils.hpp>
#include <contracts/data/data_utils.hpp>

DataTypes::HeartbeatMessage CoordinatorHeartbeat::message_;

CoordinatorHeartbeat::CoordinatorHeartbeat( contracts::IUnitContext* context
		                                      , IConnectionState* coordinator_connection)
		: connected_(false)
		, context_(context)
		, coordinator_connection_(coordinator_connection)
	{
		auto services = context_->services();
		if (services == nullptr)
			return;
		auto clients = services->clients();
		if (clients != nullptr)
			coordinator_ = clients->coordinator().get();
	}


	void CoordinatorHeartbeat::action() 	{
		if (coordinator_ == nullptr)
			return;

		coordinator_->heart_beat(message_);
		context_->logger()->info("Hearbeat");
	}

	void CoordinatorHeartbeat::init() 	{
		generate_message(*context_);

		repeatable_action_ = std::make_unique<RepeatableAction>(this, DELAY);
		repeatable_action_->start();
	}

	void CoordinatorHeartbeat::de_init() 
	{
		repeatable_action_->stop();
		context_->logger()->info("Hearbeat stopped");
	}

	void CoordinatorHeartbeat::generate_message(contracts::IUnitContext& context)
	{
		if (message_.has_id())
			return;
		auto service_id = context.configuration().service_uuid();
		auto ip_address = utils::network::get_local_ip();

		message_.set_service_type(DataTypes::ServiceType::Unit_Service);

		auto key = new DataTypes::Key();
		contracts::data::set_guid(service_id, *key);
		message_.set_allocated_id(key);
	}