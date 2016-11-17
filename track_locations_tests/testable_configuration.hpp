#ifndef TestableServiceConfiguration_Included
#define TestableServiceConfiguration_Included

#include <string>
#include <contracts/iservice_configuration.hpp>
#include <uuid/random_generator.hpp>
#include <uuid/uuid_io.hpp>

inline std::string generate_random_guid()
{
	auto guid = boost::uuids::random_generator()();
	return to_string(guid);
}

class TestableServiceConfiguration : public contracts::IServiceConfiguration	
{
public:
	TestableServiceConfiguration()
		: facial_service_address_     ("127.0.0.1:50051")
		, coordinator_service_address_("127.0.0.1:50052")
		, database_service_address_   ("127.0.0.1:49065")
		, unit_service_port_(50053)
		, service_uuid_(generate_random_guid())
	{}

	const std::string& facial_service_address() const override {
		return facial_service_address_;
	}

	const std::string& coordinator_service_address() const override {
		return coordinator_service_address_;
	}

	const std::string& database_service_address() const override {
		return database_service_address_;
	}

	uint16_t    unit_service_port() const override {
		return unit_service_port_;
	}

	const std::string&  service_uuid() const override {
		return service_uuid_;
	}

	bool empty() const
	{
		return    facial_service_address_ == ""
			&& coordinator_service_address_ == ""
			&& unit_service_port_ == 0
			&& service_uuid_ == ""
			&& database_service_address_ == "";
	}

private:
	std::string facial_service_address_;
	std::string coordinator_service_address_;
	std::string database_service_address_;
	uint16_t    unit_service_port_;
	std::string service_uuid_;


};


#endif
