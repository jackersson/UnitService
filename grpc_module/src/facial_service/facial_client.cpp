#include "facial_service/facial_client.hpp"

#include <facial_service/client/facial_client_data_api.hpp>
#include <services/service_address.hpp>
#include <facial_service/client/facial_client_calls.hpp>

namespace grpc_services
{
	FacialClient::FacialClient( contracts::services::IServiceAddress& address)
			                      : FacialClientDataApi(address)
	{
	}

	void FacialClient::do_set_call_options(services_api::IAsyncCall* call) const 
	{
		//TODO think about smarter solution
		if (call->identifier()
			== typeid(services_api::AsyncFaceProcessCall).name())
		{
			call->deadline = CONNECT_REQUEST_DEADLINE;
		}
	}
	
}
