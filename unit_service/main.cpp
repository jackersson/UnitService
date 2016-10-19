#include <string>
#include <iostream>
/*
#include <asio/io_service.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/ip/host_name.hpp>


#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <lexical_cast.hpp>
*/
#include "include/launcher.hpp"

/*
struct UnitServiceConfiguration
{
	std::string facial_service_address     ;
	std::string coordinator_service_address;
	uint16_t    unit_service_port          ;

	bool load(const std::string &filename)
	{
		using boost::property_tree::ptree;
		ptree pt;

		try
		{
			read_xml(filename, pt);
			facial_service_address
				= pt.get<std::string>("config.facial_service_address");

			coordinator_service_address
				= pt.get<std::string>("config.coordinator_service_address");

			unit_service_port = pt.get<uint16_t>("config.unit_service_port");
			return true;
		}
		catch (std::exception&)	{
			std::cout << "Loading config failed. Not found " << filename << std::endl;
			return false;
		}
	}

	bool save(const std::string &filename) const
	{
		using boost::property_tree::ptree;
		ptree pt;

	  pt.put("config.facial_service_address"     , facial_service_address);
		pt.put("config.coordinator_service_address", coordinator_service_address);
		pt.put("config.unit_service_port"          , unit_service_port);

		try
		{
			write_xml(filename, pt);
			return true;
		}
		catch (std::exception&) {
			std::cout << "Saving to config failed. Not found " << filename << std::endl;
			return false;
		}
	}

};

std::string get_local_ip()
{
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::resolver resolver(io_service);

	auto h = boost::asio::ip::host_name();
	std::cout << "Host name is " << h << '\n';	
	std::for_each(resolver.resolve({ h, "" }), {}, [](const auto& re) {
		std::string address = re.endpoint().address().to_string();
		if (address.size() <= 16)
		{
			std::cout << "ip address is : " << address << '\n';
			return address;
		}
	});

	return "0.0.0.0";
}
/*


void guid_parsing()
{
	auto uuid = boost::uuids::random_generator()();
	std::cout << uuid << std::endl;
	//"7feb24af-fc38-44de-bc38-04defc3804de"
	auto sts = boost::lexical_cast<std::string>(uuid);

	auto uu = boost::lexical_cast<boost::uuids::uuid>(sts);

	std::cout << (uu == uuid) << std::endl;
	std::cout << sts << std::endl;

}
*/
/*
int main()
{
	auto launcher = std::make_shared<Launcher>();
	launcher->init();

	std::vector<DataTypes::Location> locations;
	auto gr = new DataTypes::GetLocationRequest();
	DataTypes::GetRequest grr;
	launcher->context()->services()->clients()->database()->get(grr);
	std::cin.get();


	return 0;
}
*/
/*
class Launcher2 
{
public:
	Launcher2()
		: configuration_(new UnitServiceConfiguration(UnitServiceConfiguration::default_configuration()))
	{
	//	configuration_ = 	new UnitServiceConfiguration(UnitServiceConfiguration::default_configuration()));
	}

	virtual ~Launcher2() {
		de_init();
	}

	void init()
	{
		unit_service_.set_configuration(&configuration_);
		unit_service_.init();
	}

	void de_init()
	{
		unit_service_.de_init();
	}

	std::shared_ptr<UnitServiceConfiguration> configuration() const {
		return configuration_;
	}

	UnitService& context() {
		return unit_service_;
	}

	static const std::string CONFIG_NAME; //= "config.bio";

private:
	UnitService              unit_service_;
	std::shared_ptr<UnitServiceConfiguration> configuration_;

};

*/

void init()
{

	Launcher launcher;
	launcher.init();
	std::cin.get();
}

void test_settings()
{
	auto default_config = UnitServiceConfiguration::default_configuration();
	
	default_config.save("config.txt");

	UnitServiceConfiguration loaded;
	loaded.load("config.txt");
}


int main()
{

	init();
	
	/*
	auto gpr = new DataTypes::GetLocationRequest();
	//gpr->set_first_name("Test");
	DataTypes::GetRequest* gr = new DataTypes::GetRequest();
	gr->set_allocated_location_request(gpr);

	try
	{
		std::vector<DataTypes::Location> itms;
		auto items = launcher.context().repository()->locations()->get(gpr, itms);
		//std::cout << val->items().items().size() << std::endl;
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

//	launcher.context().repository()->locations()->get();
*/
	std::cin.get();
	


	
	//Launcher2 launcher2;
	//launcher2.init();
	/*
	std::shared_ptr<UnitServiceConfiguration> un(
		new UnitServiceConfiguration(UnitServiceConfiguration::default_configuration()));
	auto us = std::make_shared<UnitService>(un);
	us->init();
	*/
	/*
	auto launcher = std::make_shared<Launcher2>();
	launcher->init();
	*/
	/*
	auto mnger = launcher2.context().services()->clients();
	
	//auto impl = std::make_shared<grpc_services::DatabaseClientImpl>(context);


	auto gpr = new DataTypes::GetPersonRequest();
	gpr->set_first_name("Test");
	DataTypes::GetRequest gr;
	gr.set_allocated_person_request(gpr);

	try
	{
		auto val = mnger->database()->get(gr);
		//std::cout << val->items().items().size() << std::endl;
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	//TestImpl im;

	//grpc_services::ServerManager sm;
	//sm.start();

	std::cin.get();
	launcher2.de_init();
	//sm.stop();
	*/
	return 0;
}
