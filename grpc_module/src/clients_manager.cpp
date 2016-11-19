#include "clients_manager.hpp"

#include "coordinator_service/coordinator_client.hpp"
#include "database_service/database_client_data_api.hpp"
#include "database_service/datacontext/datacontext_container.hpp"

namespace grpc_services
{
	ClientManager::ClientManager(contracts::IServiceContext* context)
		: active_(false)
		, initialized_(false)
		, context_(context)
	{
		ClientManager::init();
	}

	ClientManager::~ClientManager() {
		ClientManager::de_init();
	}

	void ClientManager::start() {
		if (active_)
			return;

		for (auto it : servers_)
			it->start();

		active_ = true;
	}

	void ClientManager::stop()  {
		for (auto it : servers_)
			it->stop();
		servers_.clear();
	}
		
	void ClientManager::init() 
	{
		if (initialized_)
			return;
		auto configuration = context_->configuration();

		//database service 
		//For test, Coordinator service is going to be used
		/*
		auto address = configuration->database_service_address();
		contracts::services::ServiceAddress database_address(address);
		auto data_impl = std::make_shared<services_api::DatabaseClientDataApi>(database_address);
		servers_.push_back(data_impl.get());
		database_client_ = data_impl;
		*/

		//Coordinator client 			
		auto address = configuration->coordinator_service_address();
		contracts::services::ServiceAddress coordinator_address(address);
		auto coord_impl	= std::make_shared<CoordinatorClient>( context_->configuration()
			                                                   , coordinator_address);
		servers_.push_back(coord_impl.get());
		coordinator_client_ = coord_impl;
		database_client_    = coord_impl;

		database_context_
			= std::make_unique<services_api::datacontext::DataContextContainer>
			(database_client_.get());

		/*
		//Facial service client
		address = configuration.facial_service_address();
		contracts::services::ServiceAddress facial_service_address(address);
		facial_client_
		= std::make_unique<FacialClient>(facial_service_address);
		servers_.push_back(facial_client_.get());

		*/

		initialized_ = true;
	}

	void ClientManager::de_init() 
	{
		stop();
	}	
}

