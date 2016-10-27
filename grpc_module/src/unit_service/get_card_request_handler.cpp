
#include <unit_service/get_card_request_handler.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		void GetCardRequestHandler::ProcessRequest()
		{
			context_->logger()->info("Get card request");
			DataTypes::CardMsg response;

			if (request_.device_type() == DataTypes::DeviceType::CardReader)
			{
				auto access_device_engine = context_->devices()->access_device_engine();
				std::string result = "";
				try {
					result = access_device_engine->get_card(request_.device_name());
					response.set_card_num(result);
				}
				catch (std::exception&) {}
			}

			context_->logger()->info("Get card request done {0}", response.card_num());
			responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}