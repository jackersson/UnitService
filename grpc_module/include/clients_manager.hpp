#ifndef ClientsManager_Included
#define ClientsManager_Included

#include <contracts/iservices.hpp>
#include <memory>
#include <services/iservice.hpp>
#include "coordinator_service/coordinator_client.hpp"
#include "database_service/database_client_data_api.hpp"
#include "facial_service/facial_client.hpp"
#include "database_service/datacontext/datacontext_container.hpp"

namespace grpc_services
{
	class ClientManager : public contracts::services::IServices	                  
	{
	public:
		explicit ClientManager(contracts::IServiceContext* context)
			: active_     (false)
			, initialized_(false)
			, context_(context)
		{
			ClientManager::init();
		}

		virtual ~ClientManager() {
			ClientManager::de_init();
		}

		void start() override {
			if (active_)
				return;

			for (auto it : servers_)			
					it->start();

			active_ = true;
		}

		void stop()  override {
			for (auto it : servers_)
				it->stop();
			servers_.clear();
		}

		contracts::services::IFacialServiceApi* facial_service() override	{
			return facial_client_.get();
		}

		contracts::data::AbstractDataContextContainer* database() override {
			return database_context_.get();
		}

		contracts::services::ICoordinatorApi* coordinator() override {
			return coordinator_client_.get();
		}
		
		void init() override
		{
			if (initialized_)
				return;
			const auto& configuration = context_->configuration();		
			
			//database service 
			//For test, Database service is going to be used
			std::string address = configuration.database_service_address();
			contracts::services::ServiceAddress database_address(address);
			database_client_ 
				= std::make_unique<services_api::DatabaseClientDataApi>(database_address);
																					
			servers_.push_back(database_client_.get());

			database_context_
				= std::make_unique<services_api::datacontext::DataContextContainer>
				(database_client_.get());


			//Coordinator client 			
			address = configuration.coordinator_service_address();
			contracts::services::ServiceAddress coordinator_address(address);
			coordinator_client_ 
				= std::make_unique<CoordinatorClient>(context_, coordinator_address);
			servers_.push_back(coordinator_client_.get());			
			

			//Facial service client 	
			address = configuration.facial_service_address();
			contracts::services::ServiceAddress facial_service_address(address);
			facial_client_
				= std::make_unique<FacialClient>(facial_service_address);
			servers_.push_back(facial_client_.get());

			

			initialized_ = true;
		}

		void de_init() override
		{
			stop();
		}

	private:
		bool active_     ;
		bool initialized_;

		ClientManager(const ClientManager&) = delete;
		ClientManager& operator=(const ClientManager&) = delete;

		contracts::IServiceContext* context_;

		std::unique_ptr<services_api::DatabaseClientDataApi> database_client_;
		std::unique_ptr<CoordinatorClient>                coordinator_client_;
		std::unique_ptr<FacialClient>                     facial_client_     ;
		std::unique_ptr<contracts::data::AbstractDataContextContainer>
			database_context_;

		std::vector<IService*> servers_;
	};

	typedef std::shared_ptr<ClientManager> ClientManagerPtr;
}

#endif
