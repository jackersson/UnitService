#include "services_coordinator.hpp"

#include "server_manager.hpp"
#include "clients_manager.hpp"

namespace grpc_services
{
	ServicesCoordinator::ServicesCoordinator(contracts::IServiceContext* context)
			: context_(context)
	{
		servers_ = std::make_unique<ServerManager>(context_);
		clients_ = std::make_unique<ClientManager>(context_);
	}

	ServicesCoordinator::~ServicesCoordinator() {
		ServicesCoordinator::de_init();
	}

	//TODO not need
	void ServicesCoordinator::start()  {}
	void ServicesCoordinator::stop()  {}

	void ServicesCoordinator::init() 
	{
		clients_->start();
		servers_->start();
	}

	void ServicesCoordinator::de_init() 
	{
		clients_->stop();
		servers_->stop();
	}
}
