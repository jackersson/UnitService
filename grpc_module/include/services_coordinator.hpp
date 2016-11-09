#ifndef ServiceCoordinator_Incuded
#define ServiceCoordinator_Incuded

#include <contracts/iservice_context.hpp>
#include <contracts/iservices.hpp>
#include "server_manager.hpp"
#include "clients_manager.hpp"

namespace grpc_services
{
	class ServicesCoordinator : public contracts::services::IServices
	{
	public:
		explicit ServicesCoordinator(contracts::IServiceContext* context)			
			: context_(context)
		{
			servers_ = std::make_shared<ServerManager>(context_);
			clients_ = std::make_shared<ClientManager>(context_);
		}

		~ServicesCoordinator(){
			ServicesCoordinator::de_init();
		}

		//TODO not need
		void start() override{}
		void stop() override{}

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


		contracts::data::AbstractDataContextContainer* database() override {
			return clients_->database();
		}

		contracts::services::IFacialServiceApi* facial_service() override {
			return clients_->facial_service();
		}

		contracts::services::ICoordinatorApi* coordinator() override {
			return clients_->coordinator();
		}


	private:
		contracts::IServiceContext* context_;
		ServerManagerPtr servers_;
		ClientManagerPtr clients_;
	};
}

#endif