#ifndef ServerContext_INCLUDED
#define ServerContext_INCLUDED

#include <grpc++/grpc++.h>
#include <contracts/services/service_address.hpp>
#include <contracts/iunit_context.hpp>

namespace grpc_services
{
	class ServerContext
	{
	public:
		explicit ServerContext(  contracts::services::ServiceAddress address
			                     , std::shared_ptr<grpc::ServerBuilder> builder
		                       , contracts::IUnitContextPtr context)
			: server_builder_(builder)
			, address_(address)
			, context_(context)
		{	}

		std::shared_ptr<grpc::ServerBuilder> ServerBuilder() const		{
			return server_builder_;
		}

		const contracts::services::IServiceAddress& Address() const {
			return address_;
		}

		contracts::IUnitContextPtr unit_context() const {
			return context_;
		}

	private:
		std::shared_ptr<grpc::ServerBuilder> server_builder_;
		contracts::services::ServiceAddress  address_;
		contracts::IUnitContextPtr           context_;
	};
}

#endif