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
		                     , contracts::IUnitContext* context)
			 : address_(address)
			 , context_(context)
		{	}

		const contracts::services::IServiceAddress& address() const {
			return address_;
		}

		contracts::IUnitContext* unit_context() const {
			return context_;
		}

	private:
		contracts::services::ServiceAddress  address_;
		contracts::IUnitContext*           context_;
	};
}

#endif