#ifndef ServerManager_Included
#define ServerManager_Included

#include <memory>
#include <services/iservice.hpp>
#include <contracts/iservice_context.hpp>

namespace grpc {
	class Server;
}

namespace grpc_services
{
	class ServerManager : public contracts::services::IService
	{
	public:
		explicit ServerManager(contracts::IServiceContext* context);
		
		virtual ~ServerManager();

		void start  () override;
		void stop   () override;		
		void init   () override;
		void de_init() override;

	private:
		bool active_;
		bool initialized_;

		ServerManager(const ServerManager&) = delete;
		ServerManager& operator=(const ServerManager&) = delete;

		contracts::IServiceContext* context_;

		//std::unique_ptr<unit_service::UnitServiceImpl> unit_service_;
		std::unique_ptr<IService> unit_service_;
		std::unique_ptr<grpc::Server>  server_;
		std::vector<IService*>         servers_;

	};

	typedef std::shared_ptr<ServerManager> ServerManagerPtr;
}

#endif
