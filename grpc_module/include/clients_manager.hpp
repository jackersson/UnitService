#ifndef ClientsManager_Included
#define ClientsManager_Included

#include <contracts/services/iserver_manager.hpp>
#include <memory>
#include <contracts/services/iservice.hpp>
#include "server_context.hpp"
#include "../database_client_impl.hpp"

namespace grpc_services
{

	class ClientManager : public contracts::services::IServiceManager
	                  	, public contracts::services::IClients
	{
	public:
		explicit ClientManager(contracts::IUnitContextPtr context)
			: context_(context)
		{
			init();
		}

		virtual ~ClientManager() {
			ClientManager::stop();
		}

		void start() override {
			for (auto it = servers_.begin(); it != servers_.end(); ++it)
				it->get()->start();
		}

		void stop()  override {
			for (auto it : servers_)
				it->stop();
			servers_.clear();
		}

		contracts::services::IDatabaseApiPtr database() override {
			return database_client_;
		}

	private:
		void init()
		{
			//database service
			contracts::services::ServiceAddress address("localhost", 49065);
			database_client_ = std::make_shared<DatabaseClientImpl>(
			                                   	ClientContext(address, context_));

			servers_.push_back(database_client_);
		}


		ClientManager(const ClientManager&) = delete;
		ClientManager& operator=(const ClientManager&) = delete;

		contracts::IUnitContextPtr context_;

		std::shared_ptr<DatabaseClientImpl> database_client_;

		std::vector<std::shared_ptr<contracts::services::IService>> servers_;
	};

	typedef std::shared_ptr<ClientManager> ClientManagerPtr;
}

#endif
