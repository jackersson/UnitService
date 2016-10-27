#ifndef CoordinatorHeartbeat_Included
#define CoordinatorHeartbeat_Included

#include <contracts/iunit_context.hpp>
#include "repeatable_action.hpp"
#include "iconnection_state.hpp"


class CoordinatorHeartbeat : public IRepeatableAction
	                         , public contracts::common::IModule
{
public:
	explicit CoordinatorHeartbeat( contracts::IUnitContext* context
		                           , IConnectionState* coordinator_connection);
	~CoordinatorHeartbeat() {
		CoordinatorHeartbeat::de_init();
	}

	void action() override;

	bool can() override{
		return coordinator_connection_ == nullptr 
			? false : coordinator_connection_->connected();
	}

	void init()    override;
	void de_init() override;

	static void generate_message(contracts::IUnitContext& context);

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