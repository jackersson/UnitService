#ifndef UnitServiceConfiguration_Included
#define UnitServiceConfiguration_Included
#include <string>
#include <property_tree/ptree_fwd.hpp>
#include <property_tree/xml_parser.hpp>
#include <iostream>
#include <contracts/iunit_context.hpp>

class UnitServiceConfiguration : contracts::IUnitConfiguration
{
public:
	UnitServiceConfiguration()
		: facial_service_address_("")
		, coordinator_service_address_("")
		, unit_service_port_(0)
	{}

	const std::string& facial_service_address() override	{
		return facial_service_address_;
	}

	const std::string& coordinator_service_address() override	{
		return coordinator_service_address_;
	}

	uint16_t    unit_service_port() override	{
		return unit_service_port_;
	}

	void set_facial_service_address(const std::string& value)  {
		 facial_service_address_ = value;
	}

	void set_coordinator_service_address(const std::string& value)  {
		coordinator_service_address_ = value;
	}

	void    set_unit_service_port(uint16_t value)  {
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
		config.set_coordinator_service_address("127.0.0.1:50052");
		config.set_unit_service_port(50053);
		return config;
	}

	bool load(const std::string &filename)
	{
		using boost::property_tree::ptree;
		ptree pt;

		try
		{
			read_xml(filename, pt);
			facial_service_address_
				= pt.get<std::string>("config.facial_service_address");

			coordinator_service_address_
				= pt.get<std::string>("config.coordinator_service_address");

			unit_service_port_ = pt.get<uint16_t>("config.unit_service_port");
			return true;
		}
		catch (std::exception&) {
			std::cout << "Loading config file failed. File not found " << filename << std::endl;
			return false;
		}
	}

	bool save(const std::string &filename) const
	{
		using boost::property_tree::ptree;
		ptree pt;

		pt.put("config.facial_service_address"     , facial_service_address_);
		pt.put("config.coordinator_service_address", coordinator_service_address_);
		pt.put("config.unit_service_port"          , unit_service_port_);

		try
		{
			write_xml(filename, pt);
			return true;
		}
		catch (std::exception&) {
			std::cout << "Saving to config failed. File not found " << filename << std::endl;
			return false;
		}
	}

private:
	std::string facial_service_address_     ;
	std::string coordinator_service_address_;
	uint16_t    unit_service_port_          ;

};


#endif
