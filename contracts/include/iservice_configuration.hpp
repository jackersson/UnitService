#ifndef IServiceConfiguration_Included
#define IServiceConfiguration_Included

#include <string>

namespace contracts
{
	class IServiceConfiguration
	{
	public:
		virtual ~IServiceConfiguration() {}

		virtual const std::string& facial_service_address     () const = 0;
		virtual const std::string& coordinator_service_address() const = 0;
		virtual const std::string& unit_service_address       () const = 0;
		virtual const std::string& service_uuid               () const = 0;
		virtual const std::string& database_service_address   () const = 0;
	};	
}

#endif
