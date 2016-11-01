#include <coordinator_connector.hpp>

#include <services/service_address.hpp>
#include <network_utils.hpp>
#include <data/data_utils.hpp>

DataTypes::ConnectMsg CoordinatorConnector::connect_message_;

CoordinatorConnector::CoordinatorConnector(contracts::IUnitContext* context)
	 : connected_(false)
	 , context_(context)
{
	 auto services = context_->services();
	 if (services == nullptr)
		 return;
	 auto clients = services->clients();
	 if (clients != nullptr)
		 coordinator_ = clients->coordinator();
}

 void CoordinatorConnector::action() 
 {
	 if (coordinator_ == nullptr)
		 return;
	 try
	 {
		 connected_ = coordinator_->connect_request(connect_message_);
	 }
	 catch (std::exception& exception)
	 {
		 logger_.info("Coordinator connection error {0}"
			 , exception.what());
	 }
	 if (connected_)
	 {
		 logger_.info("Connected to coordinator successfully");
		 repeatable_action_->stop();
	 }
	 else
		 logger_.info("Coordinator client can't connect. {0}");
 }


 void CoordinatorConnector::init()
 {
	 if (repeatable_action_ != nullptr)
		 return;

	 generate_message(*context_);
	 repeatable_action_ 
		 = std::make_unique<utils::threading::RepeatableAction>(this, DELAY);
	 repeatable_action_->start();
 }

 void CoordinatorConnector::de_init() 
 {
	 repeatable_action_->stop();
 }

 void CoordinatorConnector::generate_message(contracts::IUnitContext& context)
 {
	 if (connect_message_.has_id())
		 return;
	 auto unit_service_port = context.configuration().unit_service_port();
	 auto service_id = context.configuration().service_uuid();

	 auto ip_address = utils::network::get_local_ip();
	 contracts::services::ServiceAddress sa(ip_address, unit_service_port);

	 connect_message_.set_ip_address(sa.get());
	 connect_message_.set_service_type(DataTypes::ServiceType::Unit_Service);

	 auto key = new DataTypes::Key();
	 contracts::data::set_guid(service_id, *key);
	 connect_message_.set_allocated_id(key);
 }