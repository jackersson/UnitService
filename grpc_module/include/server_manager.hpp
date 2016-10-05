#ifndef ServerManager_Included
#define ServerManager_Included

#include <contracts/services/iserver_manager.hpp>
#include <memory>
#include <grpc++/grpc++.h>
#include <contracts/services/iservice.hpp>
#include "service_context.hpp"
#include "unit_service_impl.hpp"

namespace grpc_services
{
	class ServerManager : public contracts::services::IServiceManager
	{
	public:
		explicit ServerManager()
		{
			//Init();
		}
		
		virtual ~ServerManager() {
			ServerManager::Stop(); }

		void Init()
		{
			auto builder_ = std::make_shared<grpc::ServerBuilder>();

			contracts::services::ServiceAddress sa("0.0.0.0",50045);
			builder_->AddListeningPort(sa.FormattedAddress()
				, grpc::InsecureServerCredentials());

			auto unit_service = std::make_shared<UnitServiceImpl>(ServiceContext(sa, builder_));
			servers_.push_back(unit_service);

			auto server_ = builder_->BuildAndStart();
		//	if (server_ != nullptr)
			//	server_->Shutdown();
			/*
			builder_ = std::make_shared<grpc::ServerBuilder>();

			//Unit service
			contracts::services::ServiceAddress sa("[::]:50045");
			auto unit_service = std::make_shared<UnitServiceImpl>(ServiceContext(sa, builder_));
			servers_.push_back(unit_service);

			std::string st = sa.FormattedAddress();
			std::string sm(sa.FormattedAddress());
			builder_->AddListeningPort(sm
				, grpc::InsecureServerCredentials());

			//service_ = std::make_shared<Services::UnitService::AsyncService>();
			//builder_->RegisterService(service_.get());

			//for (auto it = servers_.begin(); it != servers_.end(); ++it)
				//		it->get()->Start();

			server_ = builder_->BuildAndStart();
			if (server_!= nullptr)
			  server_->Shutdown();
				*/
		}

		void Start() override {
			Init();
			for (auto it = servers_.begin(); it != servers_.end(); ++it)
				it->get()->Start();
		}

		void Stop()  override {
			server_->Shutdown();
			for (auto it : servers_)
				it->Stop();
			servers_.clear();
		}
		
	private:
		ServerManager(const ServerManager&) = delete;
		ServerManager& operator=(const ServerManager&) = delete;

		std::unique_ptr<grpc::Server>        server_;
		std::shared_ptr<grpc::ServerBuilder> builder_;
		std::vector<std::shared_ptr<contracts::services::IService>> servers_;

	};
}

#endif
