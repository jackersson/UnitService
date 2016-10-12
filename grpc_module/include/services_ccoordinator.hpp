#ifndef ServiceCoordinator_Incuded
#define ServiceCoordinator_Incuded

#include <grpc++/grpc++.h>
#include <contracts/iunit_context.hpp>
#include <contracts/services/iserver_manager.hpp>
#include "server_manager.hpp"
#include "clients_manager.hpp"

namespace grpc_services
{
	class ServicesCoordinator : public contracts::services::IServices
	{
	public:
		explicit ServicesCoordinator(contracts::IUnitContextPtr context)			
			: context_(context)
		{
			servers_ = std::make_shared<ServerManager>();
			clients_ = std::make_shared<ClientManager>(context_);
		}

		~ServicesCoordinator(){
			ServicesCoordinator::de_init();
		}

		void init() override
		{
			clients_->start();
			servers_->start();
		}

		void de_init() override
		{
			clients_->stop();
			servers_->stop();
		}

		contracts::services::IClientsPtr clients() override	{
			return clients_;
		}

	private:
		contracts::IUnitContextPtr context_;
		ServerManagerPtr servers_;
		ClientManagerPtr clients_;
	};
}

#endif