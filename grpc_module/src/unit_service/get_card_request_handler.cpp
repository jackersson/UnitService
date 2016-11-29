#include "unit_service/get_card_request_handler.hpp"
#include <boost/thread/future.hpp>

#include <data/models/devices.hpp>

using namespace devices;

namespace grpc_services
{
	namespace unit_service
	{
		GetCardRequestHandler::GetCardRequestHandler(Services::UnitService::AsyncService* service
			, grpc::ServerCompletionQueue* completion_queue
			, contracts::IServiceContext* context)
			: RequestHandler<Services::UnitService::AsyncService>(service, completion_queue)
			, initialized_(false)
			, responder_  (&server_context_)
			, context_    (context)
		{		
			proceed();
		}

		void GetCardRequestHandler::process_request()
		{
			logger_.info("Get card request -> in");

			if (!try_resolve_dependencies())
			{
				complete(DataTypes::CardMsg());
				return;
			}

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

		bool GetCardRequestHandler::try_resolve_dependencies()
		{
			if (initialized_)
				return initialized_;

			try
			{
				initialized_ = true;

				if (context_ == nullptr)
					throw std::exception("Context can't be null");
				auto devices = context_->devices();
				if (devices == nullptr)
					throw std::exception("Devices can't be null");

				auto engine_ = devices->access_device_engine();
				if (engine_ == nullptr)
					throw std::exception("Access Device engine can't be null");

				card_api_ = std::make_unique<GetCardApi>(engine_);
				return true;
			}
			catch (std::exception& ex)
			{
				logger_.error("GetCardRequestHandler::try_resolve_dependencies {0}", ex.what());
				return false;
			}
		}
	}
}