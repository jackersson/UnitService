#ifndef CoordinatorClient_INCLUDED
#define CoordinatorClient_INCLUDED

#include <coordinator_service/coordinator_client_data_api.hpp>
#include <iservice_context.hpp>

namespace grpc_services
{
	class CoordinatorClient : public services_api::CoordinatorClientDataApi
	{
	public:
		explicit CoordinatorClient( contracts::IServiceConfiguration* context
			                        , contracts::services::IServiceAddress& address);
	
		void do_set_call_options(services_api::IAsyncCall* call) const override;
		
	private:
		CoordinatorClient(const CoordinatorClient&) = delete;
		CoordinatorClient& operator=(const CoordinatorClient&) = delete;

		contracts::IServiceConfiguration* configuration_;
		static const int CONNECT_REQUEST_DEADLINE;
	};
}

#endif