#ifndef ServerContext_INCLUDED
#define ServerContext_INCLUDED

#include <grpc++/grpc++.h>
#include <services/service_address.hpp>
#include <contracts/iunit_context.hpp>

namespace grpc_services
{
	class ServerContext
	{
	public:
		explicit ServerContext(  contracts::services::ServiceAddress address
			                     , std::shared_ptr<grpc::ServerBuilder> builder
		                       , contracts::IUnitContext* context)
			: server_builder_(builder)
			, address_(address)
			, context_(context)
		{	}

		std::shared_ptr<grpc::ServerBuilder> server_builder() const		{
			return server_builder_;
		}

		const contracts::services::IServiceAddress& address() const {
			return address_;
		}

		contracts::IUnitContext* unit_context() const {
			return context_;
		}

	private:
		std::shared_ptr<grpc::ServerBuilder> server_builder_;
		contracts::services::ServiceAddress  address_;
		contracts::IUnitContext*           context_;
	};
}

#endif