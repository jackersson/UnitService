#ifndef ServerManager_Included
#define ServerManager_Included

#include <memory>
#include <grpc++/grpc++.h>
#include <services/iservice.hpp>
#include "unit_service/unit_service_impl.hpp"
#include <contracts/iunit_context.hpp>

namespace grpc_services
{
	class ServerManager : public contracts::services::IService
	{
	public:
		explicit ServerManager(contracts::IUnitContext* context)
			: active_(false)
			, initialized_(false)
			, context_(context)
		{
			ServerManager::init();
		}
		
		virtual ~ServerManager() {
			ServerManager::de_init();
		}

		void start() override {
			if (active_)
				return;

			active_ = true;
			for (auto it : servers_)
				it->start();
		}

		void stop()  override {
			if (!active_)
				return;
			if (server_ != nullptr )
		  	server_->Shutdown();

			for (auto it : servers_)
			  it->stop();
			servers_.clear();
		}
		
		void init() override
		{
			if (initialized_)
				return;

			ServerBuilder builder;
			auto port    = context_->configuration().unit_service_port();
			
			//Unit service
			contracts::services::ServiceAddress sa("0.0.0.0", port);
			unit_service_ = std::make_unique<unit_service::UnitServiceImpl>(
				sa, &builder, context_);
			servers_.push_back(unit_service_.get());

			server_ = builder.BuildAndStart();
			initialized_ = true;
		}

		void de_init() override
		{
			stop();
		}

		bool active_;
		bool initialized_;

		ServerManager(const ServerManager&) = delete;
		ServerManager& operator=(const ServerManager&) = delete;

		contracts::IUnitContext* context_;

		std::unique_ptr<unit_service::UnitServiceImpl> unit_service_;

		std::unique_ptr<grpc::Server>        server_;
		std::vector<IService*> servers_;

	};

	typedef std::shared_ptr<ServerManager> ServerManagerPtr;
}

#endif
