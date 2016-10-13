#include <string>
#include <iostream>
#include <asio/io_service.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/ip/host_name.hpp>


#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <lexical_cast.hpp>
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
*/
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
void test_settings()
{
	UnitServiceConfiguration default_config;
  default_config.coordinator_service_address = "127.0.0.1:50051";
  default_config.facial_service_address      = "127.0.0.1:50052";
  default_config.unit_service_port           = 50053;
  
  default_config.save("config.txt");
  
  UnitServiceConfiguration loaded;
  loaded.load("config.txt");
}
*/
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

int main()
{
	Launcher launcher;
	launcher.init();
	std::cin.get();	
	
	return 0;
}
