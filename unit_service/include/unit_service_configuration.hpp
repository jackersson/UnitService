#ifndef UnitServiceConfiguration_Included
#define UnitServiceConfiguration_Included
#include <string>
#include <property_tree/ptree_fwd.hpp>
#include <property_tree/json_parser.hpp>
#include <iostream>
#include <contracts/iunit_context.hpp>
#include <data_utils.hpp>
#include <contracts/data/data_utils.hpp>

class UnitServiceConfiguration : public contracts::IUnitConfiguration
{
public:
	UnitServiceConfiguration()
		: facial_service_address_     ("")
		, coordinator_service_address_("")
		, unit_service_port_(0)
		, service_uuid_     ("")
	{}

	UnitServiceConfiguration(const UnitServiceConfiguration &obj)
		: facial_service_address_     (obj.facial_service_address_)
		, coordinator_service_address_(obj.coordinator_service_address_)
		, unit_service_port_(obj.unit_service_port_)
		, service_uuid_     (obj.service_uuid_)
	{
	}

	const std::string& facial_service_address() const override	{
		return facial_service_address_;
	}

	const std::string& coordinator_service_address() const override	{
		return coordinator_service_address_;
	}

	const std::string& database_service_address() const override {
		return coordinator_service_address_;
	}

	uint16_t    unit_service_port() const override	{
		return unit_service_port_;
	}

	const std::string&  service_uuid() const override {
		return service_uuid_;
	}

	void set_facial_service_address(const std::string& value)  {
		 facial_service_address_ = value;
	}

	void set_coordinator_service_address(const std::string& value)  {
		coordinator_service_address_ = value;
	}

	void set_database_service_address(const std::string& value) {
		database_service_address_ = value;
	}

	void    set_unit_service_port(uint16_t value)  {
		unit_service_port_ = value;
	}

	void    set_service_uuid(const std::string& value) {
		service_uuid_ = value;
	}

	bool empty() const
	{
		return    facial_service_address_      == "" 
			     && coordinator_service_address_ == ""
			     && unit_service_port_           == 0
			     && service_uuid_                == "";
	}

	static UnitServiceConfiguration default_configuration()
	{
		UnitServiceConfiguration config;
		config.set_facial_service_address     ("127.0.0.1:50051");
		config.set_coordinator_service_address("127.0.0.1:49065");
		config.set_da("127.0.0.1:49065");
		config.set_unit_service_port(50053);
		config.set_service_uuid(contracts::data::get_random_guid());
		return config;
	}

	bool load(const std::string &filename)
	{
		using boost::property_tree::ptree;
		ptree pt;

		try
		{
			read_json(filename, pt);
			facial_service_address_
				= pt.get<std::string>("facial_service_address");

			coordinator_service_address_
				= pt.get<std::string>("coordinator_service_address");

			database_service_address_
				= pt.get<std::string>("database_service_address");

			unit_service_port_ = pt.get<uint16_t>("unit_service_port");

			service_uuid_
				= pt.get<std::string>("service_uuid");
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

		//TODO to consts
		pt.put("facial_service_address"     , facial_service_address_     );
		pt.put("coordinator_service_address", coordinator_service_address_);
		pt.put("unit_service_port"          , unit_service_port_          );
		pt.put("service_uuid"               , service_uuid_               );
		try
		{
			write_json(filename, pt);
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
	std::string database_service_address_   ;
	uint16_t    unit_service_port_          ;
	std::string service_uuid_;

};


#endif
