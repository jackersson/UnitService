#ifndef ClientContext_INCLUDED
#define ClientContext_INCLUDED

#include <contracts/services/service_address.hpp>
#include <contracts/iunit_context.hpp>

namespace grpc_services
{
	class ClientContext
	{
	public:
		explicit ClientContext(const contracts::services::ServiceAddress& address
		                     , contracts::IUnitContextPtr context)
			 : address_(address)
			 , context_(context)
		{	}

		//TODO rename lower case 
		const contracts::services::IServiceAddress& Address() const {
			return address_;
		}

		contracts::IUnitContextPtr unit_context() const {
			return context_;
		}

	private:
		contracts::services::ServiceAddress  address_;
		contracts::IUnitContextPtr           context_;
	};
}

#endif