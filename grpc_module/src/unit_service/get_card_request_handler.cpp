#include "unit_service/get_card_request_handler.hpp"
#include <future>

#include <data/models/devices.hpp>

using namespace contracts::devices;

namespace grpc_services
{
	namespace unit_service
	{
		void GetCardRequestHandler::process_request()
		{
			logger_.info("Get card request");
			DataTypes::CardMsg response;

			if (request_.device_type() == DataTypes::DeviceType::CardReader)
			{

				data_model::DeviceId dev("any"
					                      , static_cast<uint16_t>(request_.serial_number()));

			//	testable_unit_context::GetCardApi card_api(engine_);
				//auto f = card_api.get_card(dev);
			
			}
			else
			{
				response.set_card_num("Device type not valid. Should be Access Device");
				complete(response);
			}
		}

		void GetCardRequestHandler::on_error(const DeviceException& exception) {}
		void GetCardRequestHandler::on_state(const IDeviceState& state)	{}

		void GetCardRequestHandler::on_next(const access_device::ICommandResult& data)
		{
			if (data.device_module() == access_device::Dallas)
			{
				DataTypes::CardMsg response;
				response.set_card_num(data.get_dallas_key());
			}
		}

		void GetCardRequestHandler::complete(const DataTypes::CardMsg& response)
		{
			engine_->remove(data_model::DeviceId("any"
				             , static_cast<uint16_t>(request_.serial_number())));
			engine_->unsubscribe(this);

			logger_.info("Get card request done {0}", response.card_num());
			responder_.Finish(response, grpc::Status::OK, this);
			next();
		}

		void GetCardRequestHandler::wait_for_complete_request()
		{
			auto handle = std::async(std::launch::async,
				[this]()
			{
				std::this_thread::sleep_for(REQUEST_TIMEOUT);
				if (this == nullptr || this->status_ != RequestStatus::FINISH)
				{
					DataTypes::CardMsg response;
					response.set_card_num("Card request timeot exception");
					complete(response);
				}
			});
		}
	}
}