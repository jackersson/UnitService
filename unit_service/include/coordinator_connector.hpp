#ifndef CoordinatorConnector_Included
#define CoordinatorConnector_Included

#include <services/icoordinator_api.hpp>
#include <contracts/iunit_context.hpp>
#include "repeatable_action.hpp"
#include <common/ilifecycle.hpp>
#include <chrono>
#include <common/logger.hpp>
#include "iconnection_state.hpp"


class CoordinatorConnector : public utils::threading::IRepeatableAction
	                         , public contracts::common::IModule
	                         , public IConnectionState
{
public:
	explicit CoordinatorConnector(contracts::IUnitContext* context);

	~CoordinatorConnector()	{
		CoordinatorConnector::de_init();
	}
		
	void action() override;
	
	bool can() override {
		return !connected_;
	}

	void init() override;
	
	void de_init() override;	
	
	bool connected() const override	{
		return connected_;
	}

private:
	static void generate_message(contracts::IUnitContext& context);

	bool connected_;
	contracts::services::ICoordinatorApi* coordinator_;
	contracts::IUnitContext* context_;

	std::unique_ptr<utils::threading::RepeatableAction> repeatable_action_;

	const std::chrono::seconds DELAY = std::chrono::seconds(3);
	static DataTypes::ConnectMsg connect_message_;

	contracts::logging::Logger logger_;
};

#endif