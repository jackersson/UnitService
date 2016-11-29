#include "server_manager.hpp"

#include <memory>
#include <grpc++/grpc++.h>
#include "unit_service/unit_service_impl.hpp"

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

		if (servers_.size() <= 0)
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
		const auto& address = context_->configuration()->unit_service_address();

		//Unit service
		contracts::services::ServiceAddress sa(address);
		unit_service_ = std::make_unique<unit_service::UnitServiceImpl>(
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
