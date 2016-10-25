#ifndef CoordinatorHeartbeat_Included
#define CoordinatorHeartbeat_Included

#include <contracts/iunit_context.hpp>
#include "repeatable_action.hpp"
#include <network_utils.hpp>
#include <contracts/data/data_utils.hpp>
#include "iconnection_state.hpp"


class CoordinatorHeartbeat : public IRepeatableAction
	                         , public contracts::common::IModule
{
public:
	explicit CoordinatorHeartbeat( contracts::IUnitContext* context
	                             , IConnectionState* coordinator_connection)
		                           : connected_(false)
		                           , context_  (context)
		                           , coordinator_connection_(coordinator_connection)
	{
		auto services = context_->services();
		if (services == nullptr)
			return;
		auto clients = services->clients();
		if (clients != nullptr)
			coordinator_ = clients->coordinator().get();
	}

	~CoordinatorHeartbeat() {
		CoordinatorHeartbeat::de_init();
	}

	void action() override
	{
		if (coordinator_ == nullptr)
			return;

		coordinator_->heart_beat(message_);
	}

	bool can() override {
		return coordinator_connection_ == nullptr 
			? false : coordinator_connection_->connected();
	}

	void init() override
	{
		generate_message(*context_);

		repeatable_action_ = std::make_unique<RepeatableAction>(this, DELAY);
		repeatable_action_->start();
	}

	void de_init() override
	{
		repeatable_action_->stop();
		context_->logger()->info("Hearbeat stopped");
	}

	static void generate_message(contracts::IUnitContext& context)
	{
		if (message_.has_id())
			return;
		auto service_id        = context.configuration().service_uuid();
		context.logger()->info("Try connect to coordinator");

		auto ip_address = utils::network::get_local_ip();

		message_.set_service_type(DataTypes::ServiceType::Unit_Service);

		auto key = new DataTypes::Key();
		contracts::data::set_guid(service_id, *key);
		message_.set_allocated_id(key);
	}

private:
	bool connected_;
	contracts::services::IHeartbeat* coordinator_;
	contracts::IUnitContext* context_;

	IConnectionState* coordinator_connection_;
	const std::chrono::seconds DELAY = std::chrono::seconds(3);

	std::unique_ptr<RepeatableAction> repeatable_action_;
	static DataTypes::HeartbeatMessage message_;
};

#endif