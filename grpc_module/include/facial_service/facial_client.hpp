#ifndef FacialClient_INCLUDED
#define FacialClient_INCLUDED

#include <facial_service/client/facial_client_data_api.hpp>

namespace grpc_services
{
	class FacialClient : public services_api::FacialClientDataApi
	{
	public:
		explicit FacialClient(contracts::services::IServiceAddress& address);	

		void do_set_call_options(services_api::IAsyncCall* call) const override;
		
	private:
		FacialClient(const FacialClient&) = delete;
		FacialClient& operator=(const FacialClient&) = delete;

		const int CONNECT_REQUEST_DEADLINE = 2;
	};
}

#endif