#ifndef CoordinatorConnector_Included
#define CoordinatorConnector_Included

#include <contracts/iservice_context.hpp>
#include <repeatable_action.hpp>
#include <logging/logger.hpp>


class CoordinatorConnector : public utils::threading::IRepeatableAction
	                         , public contracts::common::IModule
{
public:
	explicit
		CoordinatorConnector(contracts::services::ICoordinatorApi*
			                  , contracts::IServiceConfiguration*     );

	~CoordinatorConnector() {
		CoordinatorConnector::de_init();
	}

	bool try_connect();


	void action() override;

	bool can() override {
		return true;
	}

	void init() override;

	void de_init() override;

	bool connected() const {
		return connected_;
	}

private:

	const data_model::ConnectMsg&       connect_msg  () const;
	const data_model::HeartbeatMessage& heartbeat_msg() const;

	int retries_count;
	const int max_retries_count = 3;
	bool connected_;
	contracts::services::ICoordinatorApi* coordinator_;
	contracts::IServiceConfiguration*     configuration_;

	std::unique_ptr<utils::threading::RepeatableAction> repeatable_action_;

	//TODO maybe to config
	const std::chrono::seconds DELAY = std::chrono::seconds(5);

	contracts::logging::Logger logger_;
};

namespace data_model {
	enum ServiceType;
}

data_model::ConnectMsg
generate_connect_msg(uint16_t service_port
	, const std::string& service_id
	, data_model::ServiceType service_type);


data_model::HeartbeatMessage
generate_heartbeat_msg(const std::string& service_id
	, data_model::ServiceType  service_type);

#endif
