#ifndef ClientsManager_Included
#define ClientsManager_Included

#include <contracts/services/iserver_manager.hpp>
#include <memory>
#include <contracts/services/iservice.hpp>
#include "server_context.hpp"
#include "../database_client_impl.hpp"
#include "coordinator_service/coordinator_client_impl.hpp"

namespace grpc_services
{

	class ClientManager : public contracts::services::IServiceManager
	                  	, public contracts::services::IClients
	{
	public:
		explicit ClientManager(contracts::IUnitContext* context)
			: context_(context)
		{
			init();
		}

		virtual ~ClientManager() {
			ClientManager::stop();
		}

		void start() override {
			for (auto it = servers_.begin(); it != servers_.end(); ++it)
			{
				auto client = it->get();
				if ( client != nullptr)
					client->start();
			}
		}

		void stop()  override {
			for (auto it : servers_)
				it->stop();
			servers_.clear();
		}

		contracts::services::IDatabaseApiPtr database() override {
			return database_client_;
		}

		contracts::services::ICoordinatorApiPtr coordinator() override {
			return coordinator_client_;
		}

	private:
		void init()
		{
			auto coordinator_address = context_->configuration().coordinator_service_address();

			/*
			//database service 
			//For test, Database service is going to be used
			contracts::services::ServiceAddress address(coordinator_address);
			database_client_ = std::make_shared<DatabaseClientImpl>(
			                                   	ClientContext(address, context_));
																					*/
			//servers_.push_back(database_client_);


			//Coordinator client 
			contracts::services::ServiceAddress address(coordinator_address);
			coordinator_client_ = std::make_shared<CoordinatorClientImpl>(
				                                      ClientContext(address, context_));

			servers_.push_back(coordinator_client_);
		}


		ClientManager(const ClientManager&) = delete;
		ClientManager& operator=(const ClientManager&) = delete;

		contracts::IUnitContext* context_;

		std::shared_ptr<DatabaseClientImpl>    database_client_   ;
		std::shared_ptr<CoordinatorClientImpl> coordinator_client_;

		std::vector<std::shared_ptr<contracts::services::IService>> servers_;
	};

	typedef std::shared_ptr<ClientManager> ClientManagerPtr;
}

#endif
