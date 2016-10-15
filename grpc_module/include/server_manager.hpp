#ifndef ServerManager_Included
#define ServerManager_Included

#include <contracts/services/iserver_manager.hpp>
#include <memory>
#include <grpc++/grpc++.h>
#include <contracts/services/iservice.hpp>
#include "server_context.hpp"
#include "unit_service/unit_service_impl.hpp"
#include <contracts/iunit_context.hpp>

namespace grpc_services
{
	class ServerManager : public contracts::services::IServiceManager
	{
	public:
		explicit ServerManager(contracts::IUnitContextPtr context )
			: context_(context)
		{
			init();
		}
		
		virtual ~ServerManager() {
			ServerManager::stop();
		}

		void start() override {
			for (auto it = servers_.begin(); it != servers_.end(); ++it)
				it->get()->start();
		}

		void stop()  override {
			server_->Shutdown();
			for (auto it : servers_)
			  it->stop();
			servers_.clear();
		}
		
	private:
		void init()
		{
			auto builder = std::make_shared<ServerBuilder>();		
			auto port    = context_->configuration()->unit_service_port();

			//Unit service
			contracts::services::ServiceAddress sa("0.0.0.0", port);
			auto unit_service = std::make_shared<UnitServiceImpl>(
				ServerContext(sa, builder, context_));
			servers_.push_back(unit_service);
			
			server_ = builder->BuildAndStart();
		}


		ServerManager(const ServerManager&) = delete;
		ServerManager& operator=(const ServerManager&) = delete;

		contracts::IUnitContextPtr context_;

		std::unique_ptr<grpc::Server>        server_;
		std::vector<std::shared_ptr<contracts::services::IService>> servers_;

	};

	typedef std::shared_ptr<ServerManager> ServerManagerPtr;
}

#endif
