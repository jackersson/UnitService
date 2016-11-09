#ifndef FacialClient_INCLUDED
#define FacialClient_INCLUDED

#include <facial_service/client/facial_client_data_api.hpp>
#include <services/service_address.hpp>

namespace grpc_services
{
	class FacialClient : public services_api::FacialClientDataApi
	{
	public:
		explicit FacialClient(contracts::services::IServiceAddress& address)
		                    	: FacialClientDataApi(address)
		{
		}

		void do_set_call_options(services_api::IAsyncCall* call) const override
		{
			//TODO think about smarter solution
			if ( call->identifier() 
				   == typeid(services_api::AsyncFaceProcessCall).name())
			{
				call->deadline = CONNECT_REQUEST_DEADLINE;
			}		
		}

	private:
		FacialClient(const FacialClient&) = delete;
		FacialClient& operator=(const FacialClient&) = delete;

		const int CONNECT_REQUEST_DEADLINE = 2;
	};
}

#endif