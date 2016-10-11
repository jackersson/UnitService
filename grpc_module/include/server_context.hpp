#ifndef ServerContext_INCLUDED
#define ServerContext_INCLUDED

#include <grpc++/grpc++.h>
#include <contracts/services/service_address.hpp>

namespace grpc_services
{
	class ServerContext
	{
	public:
		explicit ServerContext( contracts::services::ServiceAddress address
			                     , std::shared_ptr<grpc::ServerBuilder> builder)
			: server_builder_(builder)
			, address_(address)
		{	}

		std::shared_ptr<grpc::ServerBuilder> ServerBuilder() const		{
			return server_builder_;
		}

		const contracts::services::IServiceAddress& Address() const {
			return address_;
		}

	private:
		std::shared_ptr<grpc::ServerBuilder> server_builder_;
		contracts::services::ServiceAddress  address_;
	};
}

#endif