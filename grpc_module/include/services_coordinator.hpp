#ifndef ServiceCoordinator_Incuded
#define ServiceCoordinator_Incuded

#include <contracts/iservice_context.hpp>
#include <contracts/iservices.hpp>

namespace grpc_services
{
	class ServicesCoordinator : public contracts::services::IServices
	{
	public:
		explicit ServicesCoordinator(contracts::IServiceContext* context);
			
		~ServicesCoordinator(); 		

		//TODO not need
		void start  () override;
		void stop   () override;

		void init   () override;		
		void de_init() override;	

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
		std::unique_ptr<IService>   servers_;
		std::unique_ptr<IServices>  clients_;
	};
}

#endif