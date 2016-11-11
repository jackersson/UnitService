#ifndef ServiceConfiguration_Included
#define ServiceConfiguration_Included

#include <string>
#include <map>
#include <contracts/iservice_configuration.hpp>
#include <common/logger.hpp>
#include <common/ifile_io.hpp>

enum ServiceParametrs
{
	  UnspecifiedParametr
	, FacialServiceAddress
	, CoordinatorServiceAddress
	, UnitServicePort
	, ServiceId
	, DatabaseServiceAddress //TODO remove
};

class ServiceConfiguration : public contracts::IServiceConfiguration
                           , public contracts::common::IFileIO
{
	
public:	
	ServiceConfiguration()
		: facial_service_address_     ("")
		, coordinator_service_address_("")
		, database_service_address_   ("")
		, unit_service_port_ (0) 
		, service_uuid_      ("")
	{}

	ServiceConfiguration(const ServiceConfiguration &obj)
		: facial_service_address_     (obj.facial_service_address_)
		, coordinator_service_address_(obj.coordinator_service_address_)
		, database_service_address_   (obj.database_service_address_)
		, unit_service_port_          (obj.unit_service_port_)
		, service_uuid_               (obj.service_uuid_)
	{
	}
	
	const std::string& facial_service_address() const override	{
		return facial_service_address_;
	}

	const std::string& coordinator_service_address() const override	{
		return coordinator_service_address_;
	}

	const std::string& database_service_address() const override {
		return database_service_address_;
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
			     && service_uuid_                == ""
			     && database_service_address_    == "";
	}

	static ServiceConfiguration default_configuration();

	bool save(const std::string &filename) const override;
	bool load(const std::string &filename) override;	

private:
	std::string facial_service_address_     ;
	std::string coordinator_service_address_;
	std::string database_service_address_   ;
	uint16_t    unit_service_port_          ;
	std::string service_uuid_;

	
	static std::map<ServiceParametrs, std::string> parametrs_;

	mutable contracts::logging::Logger logger_;


};


#endif
