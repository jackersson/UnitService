#include <iostream>
#include <server_manager.hpp>
#include "database_client_impl.hpp"

#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>
#include <grpc/support/log.h>

#include <services/database_service.grpc.pb.h>
#include "include/clients_manager.hpp"
#include "include/services_coordinator.hpp"
#include <network_utils.hpp>
//#include <platform_includes.hpp>

#include <sstream>
#include <asio/io_service.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/ip/host_name.hpp>
#include <iostream>

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;


class UnitServiceConfiguration : public contracts::IUnitConfiguration
{
public:
	UnitServiceConfiguration()
		: facial_service_address_("")
		, coordinator_service_address_("")
		, unit_service_port_(0)
	{}

	UnitServiceConfiguration(const UnitServiceConfiguration &obj)
		: facial_service_address_(obj.facial_service_address_)
		, coordinator_service_address_(obj.coordinator_service_address_)
		, unit_service_port_(obj.unit_service_port_)
	{
		
	}

	const std::string& facial_service_address() const override {
		return facial_service_address_;
	}

	const std::string& coordinator_service_address() const override {
		coordinator_service_address_ = "127.0.0.1:65400";
		return coordinator_service_address_;
	}

	uint16_t    unit_service_port() const override {
		return unit_service_port_;
	}

	const std::string& service_uuid() const override
	{
		service_uuid_ = "2efb5dd1-5970-49b0-9f46-ec866e9282c1";
		return service_uuid_;
	}


	const std::string& database_service_address() const override {
		return "";
	}

	void set_facial_service_address(const std::string& value) const
	{
		facial_service_address_ = value;
	}

	void set_coordinator_service_address(const std::string& value) const
	{
		coordinator_service_address_ = value;
	}

	void    set_unit_service_port(uint16_t value) {
		unit_service_port_ = value;
	}

	bool empty() const
	{
		return    facial_service_address_ == ""
			&& coordinator_service_address_ == ""
			&& unit_service_port_ == 0;
	}

	static UnitServiceConfiguration default_configuration()
	{
		UnitServiceConfiguration config;
		config.set_facial_service_address("127.0.0.1:50051");
		config.set_coordinator_service_address("127.0.0.1:49065");
		config.set_unit_service_port(50053);
		return config;
	}

private:
	mutable std::string facial_service_address_;
	mutable std::string coordinator_service_address_;
	uint16_t    unit_service_port_;
	mutable std::string service_uuid_;

};

class UnitService : public contracts::common::IModule
	, public contracts::IUnitContext
	//, public std::enable_shared_from_this<contracts::IUnitContext>
{
public:
	UnitService() :	logger_(std::make_shared<contracts::common::Logger>())
		, configuration_(new UnitServiceConfiguration(UnitServiceConfiguration::default_configuration()))
	
	{
		//logger_ = std::make_shared<contracts::common::Logger>();
		//configuration_ = std::make_shared<UnitServiceConfiguration>(UnitServiceConfiguration::default_configuration());
		logger_->info("UnitService created");
		//configuration_ = std::shared_ptr<UnitServiceConfiguration>(
		//	UnitServiceConfiguration::default_configuration());
	}

	~UnitService() {
		UnitService::de_init();
		UnitService::logger()->info("Unit service destroyed");
	}

	//TODO refactor
	void init() override
	{
		logger()->info("Unit service start init");

		//auto this_ptr = shared_from_this();
		services_ = std::make_shared<grpc_services::ServicesCoordinator>(this);
		services_->init();

		//services_->clients()->coordinator()->connect();
		connect();

		logger()->info("Unit service init done");
	}

	//TODO refactor
	void de_init() override
	{
		if (services_ != nullptr)
			services_->de_init();

		if (devices_ != nullptr)
			devices_->de_init();

		if (repository_ != nullptr)
			repository_->de_init();
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

	std::shared_ptr<contracts::devices::IDevicesContainer> devices() override {
		return devices_;
	}

	std::shared_ptr<contracts::data::IRepositoryContainer> repository() override {
		return repository_;
	}

	std::shared_ptr<contracts::services::IServices>        services() override {
		return services_;
	}

	std::shared_ptr<contracts::common::Logger>             logger() override {
		return logger_;
	}

	contracts::IUnitConfiguration&       configuration() override {
		return *configuration_.get();
	}

	std::shared_ptr<contracts::locations::ITrackLocationsEngine>
		track_locations() override
	{
		return tracking_coordinator_;
	}

	std::shared_ptr<contracts::IUnitConfiguration> configuration_;

	contracts::locations::ITrackLocationsEnginePtr tracking_coordinator_;
	std::shared_ptr<contracts::services::IServices>        services_;
	std::shared_ptr<contracts::common::Logger>             logger_;
	std::shared_ptr<contracts::devices::IDevicesContainer> devices_;
	std::shared_ptr<contracts::data::IRepositoryContainer> repository_; //database service
};

/*
std::shared_ptr<contracts::services::IClients> get()
{
	auto un = std::make_shared<UnitService>();
	contracts::services::ServiceAddress address("localhost", 49065);
	grpc_services::ClientContext context(address, un);

	auto some = std::make_shared<grpc_services::ClientManager>(un);// mnger(un);
	some->start();

	return some;
}
*/
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

//bool need_to_stop_;



class Testable
{
public:
	Testable() : work(ioService)
		, need_to_stop_(false)
	{
		
	}

	void test(std::string ted)
	{
		while (!need_to_stop_)
		{
			std::cout << ted << std::endl;
		}
	}

	void start()
	{
		threadpool.create_thread(
			boost::bind(&boost::asio::io_service::run, &ioService)
		);
		//threadpool.create_thread(
		//	boost::bind(&boost::asio::io_service::run, &ioService)
		//);

		ioService.post(std::bind(&Testable::test, this, "Hello World!"));
		//ioService.post(boost::bind(test, "./cache"));
	}

	void stop()
	{
		need_to_stop_ = true;
		ioService.stop();
		threadpool.join_all();
	}

private:
	bool need_to_stop_;
	boost::asio::io_service ioService;
	boost::thread_group threadpool;
	boost::asio::io_service::work work;
};


void some_test()
{


	//Testable ts;
	//ts.start();
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//ts.stop();
	//auto us = std::make_shared<UnitService>();
	UnitService us;
	us.init();
	/*
	auto un = std::make_shared<UnitService>();
	contracts::services::ServiceAddress address("localhost", 49065);
	grpc_services::ClientContext context(address, un);

	grpc_services::ClientManager mn(un);
	//auto some = std::make_shared<grpc_services::ClientManager>(un);// mnger(un);
	mn.start();
	*/
	//contracts::services::IClients& mnger = *us.services()->clients();
	//auto impl = std::make_shared<grpc_services::DatabaseClientImpl>(context);

	/*
	auto gpr = new DataTypes::GetPersonRequest();
	gpr->set_first_name("Test");
	DataTypes::GetRequest gr;
	gr.set_allocated_person_request(gpr);

	try
	{
		auto val = mnger.database()->get(gr);
		//std::cout << val->items().items().size() << std::endl;
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	*/

	//TestImpl im;

	//grpc_services::ServerManager sm;
	//sm.start();
	//us.de_init();
	std::cin.get();
	//us.de_init();
	//sm.stop();
}

int main()
{
	some_test();
	

	return 0;
}
