#include <unit_service/get_card_request_handler.hpp>

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
				auto& dev_name = request_.device_name();
				engine_->add(dev_name);
				engine_->subscribe(this, dev_name);
				wait_for_complete_request();
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
			if (data.module() == access_device::Dallas)
			{
				DataTypes::CardMsg response;
				response.set_card_num(data.to_string());
			}
		}

		void GetCardRequestHandler::complete(const DataTypes::CardMsg& response)
		{
			engine_->remove(request_.device_name());
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