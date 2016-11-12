#include "server_manager.hpp"

#include <memory>
#include <grpc++/grpc++.h>
//#include <services/iservice.hpp>
#include "unit_service/unit_service_impl.hpp"
//#include <contracts/iservice_context.hpp>

namespace grpc_services
{
	ServerManager::ServerManager(contracts::IServiceContext* context)
			: active_(false)
			, initialized_(false)
			, context_(context)
	{
		ServerManager::init();
	}

	ServerManager::~ServerManager() {
		ServerManager::de_init();
	}

	void ServerManager::start() {
		if (active_)
			return;

		active_ = true;
		for (auto it : servers_)
			it->start();
	}

	void ServerManager::stop()  {
		if (!active_)
			return;
		if (server_ != nullptr)
			server_->Shutdown();

		for (auto it : servers_)
			it->stop();
		servers_.clear();
	}

	void ServerManager::init()
	{
		if (initialized_)
			return;

		grpc::ServerBuilder builder;
		auto port = context_->configuration().unit_service_port();

		//Unit service
		contracts::services::ServiceAddress sa("0.0.0.0", port);
		auto unit_service_ = std::make_unique<unit_service::UnitServiceImpl>(
			sa, &builder, context_);
		servers_.push_back(unit_service_.get());

		server_ = builder.BuildAndStart();
		initialized_ = true;
	}

	void ServerManager::de_init()
	{
		stop();
	}
}
