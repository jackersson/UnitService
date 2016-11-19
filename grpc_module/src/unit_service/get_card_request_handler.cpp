#include "unit_service/get_card_request_handler.hpp"
#include <boost/thread/future.hpp>

#include <data/models/devices.hpp>

using namespace contracts::devices;

namespace grpc_services
{
	namespace unit_service
	{
		void GetCardRequestHandler::process_request()
		{
			logger_.info("Get card request -> in");
			if (request_.device_type() == DataTypes::DeviceType::CardReader)
			{
				auto serial = static_cast<uint16_t>(request_.serial_number());
				data_model::DeviceId dev("any", serial);

				boost::async([this, dev]() {
					return card_api_->get_card(dev);
				})
					.then([this](boost::future<std::string> f) {
					DataTypes::CardMsg response;
					response.set_card_num(f.get());
					complete(response);
				}).get();
			}
			else
			{
				logger_.info("Get card request error : Device type not CardReader");
				complete(DataTypes::CardMsg());
			}
		}

		void GetCardRequestHandler::complete(const DataTypes::CardMsg& response)
		{		
			logger_.info("Get card request done {0}", response.card_num());
			next();
			responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}