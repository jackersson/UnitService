#include "unit_service.hpp"
#include <data_utils.hpp>
#include <network_utils.hpp>
#include <repository_container.hpp>

void UnitService::init() 
{
	logger()->info("Unit service start init");

	//auto this_ptr = shared_from_this();
	/*
	devices_ = std::make_shared<DevicesContainer>();
	devices_->init();
	*/
	services_ = std::make_shared<grpc_services::ServicesCoordinator>(this);
	modules_.push_back(services_.get());
	//services_->init();
	
	repository_ = std::make_shared<data_core::RepositoryContainer>(this);
	modules_.push_back(repository_.get());

	//repository_->init();
	/*
	tracking_coordinator_
	= std::make_shared<tracking::locations::TrackLocationCoordinator>(this_ptr);
	*/

	for (auto module : modules_)
	{
		if (module != nullptr)
			module->init();
	}

	//connect();

	logger()->info("Unit service init done");

}

void UnitService::de_init() 
{	
	for( auto module : modules_ )
	{
		if ( module != nullptr)
		  module->de_init();
	}
}

void UnitService::connect()
{
	logger()->info("Try connect to coordinator");
	
	DataTypes::ConnectMsg msg;
	auto ip_address = utils::network::get_local_ip();
	logger_->info("Pc local IP {0}", ip_address);
	contracts::services::ServiceAddress sa( ip_address
		                                 , configuration_->unit_service_port());

	msg.set_ip_address(sa.get());
	msg.set_service_type(DataTypes::ServiceType::Unit_Service);

	auto service_uuid = configuration_->service_uuid();
	auto key = new DataTypes::Key();
	contracts::data::set_guid(service_uuid, *key);
	msg.set_allocated_id(key);

	logger_->info("Unit Service UUID {0}", service_uuid);

	services_->clients()->coordinator()->connect_request(msg);
}
