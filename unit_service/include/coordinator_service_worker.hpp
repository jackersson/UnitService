#ifndef CoordinatorServiceWorker_Included
#define CoordinatorServiceWorker_Included

#include <contracts/iservice_context.hpp>

namespace services
{
	namespace helpers
	{
		class CoordinatorConnector;
		class CoordinatorHeartbeat;
	}
}

class CoordinatorServiceWorker : public contracts::common::IModule 
	                             , public contracts::services::ICoordinatorMessages
	
{
public:
	explicit CoordinatorServiceWorker(contracts::IServiceContext* context);

	~CoordinatorServiceWorker(){
		CoordinatorServiceWorker::de_init();
	}

	void init   () override;
	void de_init() override;

	const data_model::ConnectMsg&       connect_msg  () const override;
	const data_model::HeartbeatMessage& heartbeat_msg() const override;

private:
	contracts::IServiceContext* context_;

	std::unique_ptr<services::helpers::CoordinatorConnector> connector_;
	std::unique_ptr<services::helpers::CoordinatorHeartbeat> heartbeat_;
};

#endif