#ifndef ClientsManager_Included
#define ClientsManager_Included

#include <contracts/iservices.hpp>
#include <memory>
#include <services/iservice.hpp>
#include <contracts/iservice_context.hpp>
#include <services/idatabase_api.hpp>

namespace grpc_services
{
	class ClientManager : public contracts::services::IServices	                  
	{
	public:
		explicit ClientManager(contracts::IServiceContext* context);

		virtual ~ClientManager();

		void start() override;
		void stop () override;

		contracts::services::IFacialServiceApi* facial_service() override	{
			return facial_client_.get();
		}

		contracts::data::AbstractDataContextContainer* database() override {
			return database_context_.get();
		}

		contracts::services::ICoordinatorApi* coordinator() override {
			return coordinator_client_.get();
		}
		
		void init   () override;	
		void de_init() override;
	

	private:
		bool active_     ;
		bool initialized_;

		ClientManager(const ClientManager&) = delete;
		ClientManager& operator=(const ClientManager&) = delete;

		contracts::IServiceContext* context_;

		std::shared_ptr<contracts::services::IDatabaseApi>
			database_client_;

		std::shared_ptr<contracts::services::ICoordinatorApi>
			coordinator_client_;

		std::shared_ptr<contracts::services::IFacialServiceApi>
			facial_client_ ;

		std::unique_ptr<contracts::data::AbstractDataContextContainer>
			database_context_;

		std::vector<IService*> servers_;
	};

	typedef std::shared_ptr<ClientManager> ClientManagerPtr;
}

#endif
