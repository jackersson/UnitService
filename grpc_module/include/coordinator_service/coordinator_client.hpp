#ifndef CoordinatorClient_INCLUDED
#define CoordinatorClient_INCLUDED

#include <services/service_address.hpp>
#include <coordinator_service/coordinator_client_impl.hpp>
#include <network_utils.hpp>
#include <contracts/iunit_context.hpp>

namespace grpc_services
{
	class CoordinatorClient : public services_api::CoordinatorClientImpl
	{
	public:
		explicit CoordinatorClient( contracts::IUnitContext* context
			                        , contracts::services::IServiceAddress& address)
		                         	: services_api::CoordinatorClientImpl(address)
			                        , context_(context)
		{
		}

		void do_set_call_options(services_api::IAsyncCall* call) const override
		{
			//TODO think about smarter solution
			if (call->identifier()
			   	== typeid(services_api::AsyncConnectCall).name())
			{
				call->deadline = CONNECT_REQUEST_DEADLINE;
				set_connect_msg_metadata(call->metadata);
			}

		}

		//TODO to utils
		void set_connect_msg_metadata
		     (std::vector<std::pair<std::string, std::string>>& metadata) const
		{
			metadata.push_back(get_metadata_service_id());
			metadata.push_back(get_metadata_macaddress());
			metadata.push_back(get_metadata_ip        ());
		}
		//TODO to utils
		std::pair<std::string, std::string>
			get_metadata_macaddress() const
		{
			static auto mac_address = utils::network::get_mac_address();
			return std::pair<std::string, std::string>("mac", mac_address);
		}
		//TODO to utils
		std::pair<std::string, std::string>
			get_metadata_service_id() const
		{
			static auto id = context_->configuration().service_uuid();
			return std::pair<std::string, std::string>("id", id);
		}
		//TODO to utils
		std::pair<std::string, std::string>
			get_metadata_ip() const
		{
			static auto ip_address = utils::network::get_local_ip();
			return std::pair<std::string, std::string>("ip", ip_address);
		}

	private:
		CoordinatorClient(const CoordinatorClient&) = delete;
		CoordinatorClient& operator=(const CoordinatorClient&) = delete;

		contracts::IUnitContext* context_;
		const int CONNECT_REQUEST_DEADLINE = 2;
	};
}

#endif