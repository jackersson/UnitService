#ifndef CoordinatorConnector_Included
#define CoordinatorConnector_Included
#include <contracts/iunit_context.hpp>
#include "repeatable_action.hpp"
#include <contracts/services/service_address.hpp>
#include <network_utils.hpp>
#include <contracts/data/data_utils.hpp>
#include "iconnection_state.hpp"


class CoordinatorConnector : public IRepeatableAction
	                         , public contracts::common::IModule
	                         , public IConnectionState
{
public:
	explicit CoordinatorConnector( contracts::IUnitContext* context)
		                           : connected_(true)
		                           , context_(context)
	{
		auto services = context_->services();
		if (services == nullptr)
			return;
		auto clients = services->clients();
		if ( clients != nullptr)
		  coordinator_ = clients->coordinator();
	}

	void action() override
	{
		if (coordinator_ == nullptr)
			return;

	  connected_ =	coordinator_->connect_request(connect_message_);
		if (connected_)
		{
			context_->logger()->info("Connected to coordinator successfully");
			repeatable_action_->stop();
		}
		else 
			context_->logger()->info("Coordinator client can't connect. {0}");
	}

	bool can() override {
		return !connected_;
	}

	void init() override
	{
		if (repeatable_action_ != nullptr)
			return;

		generate_message(*context_);
		repeatable_action_ = std::make_unique<RepeatableAction>(this, DELAY);
		repeatable_action_->start();
	}

	void de_init() override
	{
		repeatable_action_->stop();
	}

	static void generate_message(contracts::IUnitContext& context)
	{
		if (connect_message_.has_id())
			return;
		auto unit_service_port = context.configuration().unit_service_port();
		auto service_id        = context.configuration().service_uuid();
		context.logger()->info("Try connect to coordinator");

		auto ip_address = utils::network::get_local_ip();
		contracts::services::ServiceAddress sa(ip_address, unit_service_port);

		connect_message_.set_ip_address(sa.get());
		connect_message_.set_service_type(DataTypes::ServiceType::Unit_Service);

		auto key = new DataTypes::Key();
		contracts::data::set_guid(service_id, *key);
		connect_message_.set_allocated_id(key);
	}

	bool connected() const override	{
		return connected_;
	}

private:
	bool connected_;
	contracts::services::ICoordinatorApiPtr coordinator_;
	contracts::IUnitContext* context_;

	std::unique_ptr<RepeatableAction> repeatable_action_;

	const std::chrono::seconds DELAY = std::chrono::seconds(3);
	static DataTypes::ConnectMsg connect_message_;
};

#endif