#include "service_context.hpp"

#include <repository_container.hpp>
#include <devices_container.hpp>
#include <track_locations_engine.hpp>
#include <services_coordinator.hpp>

#include "coordinator_connector.hpp"
#include <network_utils.hpp>
#include <data/data_utils.hpp>
#include <data/models/location.hpp>
#include <uuid/string_generator.hpp>
#include <uuid/uuid_io.hpp>

ServiceContext::ServiceContext() : configuration_(nullptr)                                 
{}


ServiceContext::~ServiceContext() {
	ServiceContext::de_init();
}

void ServiceContext::init()
{
	logger_.info("Unit service start init");
	
	devices_ = std::make_unique<DevicesContainer>();
	modules_.push_back(devices_.get());

	services_ = std::make_unique<grpc_services::ServicesCoordinator>(this);
	modules_.push_back(services_.get());
		
	auto database_api = services_->database();
	repository_ = std::make_unique<data_core::RepositoryContainer>(database_api);
	modules_.push_back(repository_.get());
		
	tracking_coordinator_
  	= std::make_shared<tracking::locations::TrackLocationsEngine>(this);
	modules_.push_back(tracking_coordinator_.get());	
	
	coordinator_service_ = std::make_unique<CoordinatorConnector>
		(services_->coordinator(), configuration_);
	modules_.push_back(coordinator_service_.get());

	for (auto module : modules_)
	{
		if (module != nullptr)
			module->init();
	}
	
	data_model::Location loc;
	loc.set_name("Testable");
	loc.set_unit_mac_address(utils::network::get_mac_address());

	boost::uuids::string_generator gen;
	auto u1 = gen("{9e7229b7-c22e-4e67-9d92-95751fdece22}");
	loc.set_id(data_model::Key(u1));
	std::cout << "location id" << to_string(loc.id().guid()) << std::endl;
	data_model::AccessDevice access_device(3);
	loc.set_access_device(access_device);
	tracking_coordinator_->update(loc);
	
	logger_.info("Unit service init done");
}

void ServiceContext::de_init()
{	
	for( auto module : modules_ )
	{
		if ( module != nullptr)
		  module->de_init();
	}
}
/*
const data_model::ConnectMsg& ServiceContext::connect_msg() const
{
	static auto msg = services::helpers::generate_connect_msg(
		  configuration_->unit_service_port()
		, configuration_->service_uuid()
		, data_model::ServiceType::UnitService);

	return msg;
}

const data_model::HeartbeatMessage& ServiceContext::heartbeat_msg() const
{
	static auto msg = services::helpers::generate_heartbeat_msg(
	  	configuration_->service_uuid()
		, data_model::ServiceType::UnitService);

	return msg;
}
*/


