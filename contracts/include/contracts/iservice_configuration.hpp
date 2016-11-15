#ifndef IServiceConfiguration_Included
#define IServiceConfiguration_Included

namespace contracts
{
	class IServiceConfiguration
	{
	public:
		virtual ~IServiceConfiguration() {}

		virtual const std::string& facial_service_address() const = 0;
		virtual const std::string& coordinator_service_address() const = 0;

		virtual uint16_t    unit_service_port() const = 0;

		virtual const std::string& service_uuid() const = 0;

		virtual const std::string& database_service_address() const = 0;
	};	
}

#endif
