#include "coordinator_service/coordinator_client.hpp"

#include <network_utils.hpp>
#include <contracts/iservice_context.hpp>
#include <coordinator_service/coordinator_client_calls.hpp>

namespace grpc_services
{
	 const int CoordinatorClient::CONNECT_REQUEST_DEADLINE = 2;

	//TODO to utils
	void set_connect_msg_metadata(
		std::vector<std::pair<std::string, std::string>>& metadata
	, const contracts::IServiceConfiguration& configuration);

	std::pair<std::string, std::string>	get_metadata_macaddress();

	std::pair<std::string, std::string>
			get_metadata_service_id(const contracts::IServiceConfiguration& configuration);
	
	std::pair<std::string, std::string>	get_metadata_ip();

	CoordinatorClient::CoordinatorClient(
		  contracts::IServiceConfiguration*     config
		, contracts::services::IServiceAddress& address)
		: CoordinatorClientDataApi(address)
		, configuration_(config)
	{}

	void CoordinatorClient::do_set_call_options(services_api::IAsyncCall* call) const 
	{
		//TODO think about smarter solution
		if (call->identifier()
			== typeid(services_api::AsyncConnectCall).name())
		{
			call->deadline = CONNECT_REQUEST_DEADLINE;
			set_connect_msg_metadata(call->metadata, *configuration_);
		}
	}

	
	void set_connect_msg_metadata
	( std::vector<std::pair<std::string, std::string>>& metadata
	, const contracts::IServiceConfiguration& configuration  )
	{
		metadata.push_back(get_metadata_service_id(configuration));
		metadata.push_back(get_metadata_macaddress());
		metadata.push_back(get_metadata_ip());
	}
	
	std::pair<std::string, std::string>	get_metadata_macaddress() 
	{
		static auto mac_address = utils::network::get_mac_address();
		return std::pair<std::string, std::string>("mac", mac_address);
	}

	std::pair<std::string, std::string>
		get_metadata_service_id(const contracts::IServiceConfiguration& configuration) 
	{
		static auto id = configuration.service_uuid();
		return std::pair<std::string, std::string>("id", id);
	}
	
	std::pair<std::string, std::string>	get_metadata_ip() 		
	{
		static auto ip_address = utils::network::get_local_ip();
		return std::pair<std::string, std::string>("ip", ip_address);
	}	
}
