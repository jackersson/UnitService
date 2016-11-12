#include <service_configuration.hpp>
#include <string>
#include <property_tree/ptree_fwd.hpp>
#include <property_tree/json_parser.hpp>
#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp> 
#include <boost/uuid/uuid_io.hpp>         


typedef std::pair<ServiceParametrs, std::string> ServiceParametr;

std::string generate_random_guid()
{
	auto guid = boost::uuids::random_generator()();
	return to_string(guid);
}

std::map<ServiceParametrs, std::string> ServiceConfiguration::parametrs_ =
{
	  ServiceParametr(FacialServiceAddress     , "FacialServiceAddress"     )
	, ServiceParametr(CoordinatorServiceAddress, "CoordinatorServiceAddress")
	, ServiceParametr(UnitServicePort          , "UnitServicePort"          )
	, ServiceParametr(ServiceId                , "ServiceId"                )
	, ServiceParametr(DatabaseServiceAddress   , "DatabaseServiceAddress"   )
};

ServiceConfiguration ServiceConfiguration::default_configuration()
{
	ServiceConfiguration config;
	config.set_facial_service_address     ("127.0.0.1:50051");
	config.set_coordinator_service_address("127.0.0.1:49095");
	config.set_database_service_address   ("127.0.0.1:49065");
	config.set_unit_service_port          (50053            );

	config.set_service_uuid(generate_random_guid());
	return config;
}

bool ServiceConfiguration::load(const std::string &filename)
{
	using boost::property_tree::ptree;
	ptree pt;

	try
	{
		read_json(filename, pt);
		facial_service_address_
			= pt.get<std::string>(parametrs_[FacialServiceAddress]);

		coordinator_service_address_
			= pt.get<std::string>(parametrs_[CoordinatorServiceAddress]);

		database_service_address_
			= pt.get<std::string>(parametrs_[DatabaseServiceAddress]);

		unit_service_port_ = pt.get<uint16_t>(parametrs_[UnitServicePort]);

		service_uuid_
			= pt.get<std::string>(parametrs_[ServiceId]);
		return true;
	}
	catch (std::exception& ex) {
		logger_.error( "Loading config file failed. File not found {0}. Error {1}"
			           , filename, ex.what());
		return false;
	}
}

bool ServiceConfiguration::save(const std::string &filename) const
{
	using boost::property_tree::ptree;
	ptree pt;

	pt.put(parametrs_[FacialServiceAddress]     , facial_service_address_     );
	pt.put(parametrs_[CoordinatorServiceAddress], coordinator_service_address_);
	pt.put(parametrs_[DatabaseServiceAddress]   , coordinator_service_address_);
	pt.put(parametrs_[UnitServicePort]          , unit_service_port_          );
	pt.put(parametrs_[ServiceId]                , service_uuid_               );

	try
	{
		write_json(filename, pt);
		return true;
	}
	catch (std::exception& ex) {
		logger_.error("Saving to config failed. File not found {0}. Error {1}"
			            ,filename, ex.what());
		return false;
	}
}

