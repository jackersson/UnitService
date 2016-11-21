#include "unit_service/update_location_request_handler.hpp"

#include <helpers/request_adapters.hpp>
#include <data/models/location.hpp>
#include <future>

using namespace services_api::helpers;

namespace grpc_services
{
	namespace unit_service
	{
		void UpdateLocationRequestHandler::process_request()
		{
			logger_.info("Update location -> in");
			google::protobuf::Empty response;

			next();
			responder_.Finish(response, grpc::Status::OK, this);

			std::async(std::launch::async, [this]() {
				try
				{
					for (const auto& item : request_.items())
						update(item);
				}
				catch (std::exception& ex)
				{
					logger_.error(ex.what());
				}
			});

			logger_.info("Update location -> out ok");
		}

		void UpdateLocationRequestHandler::update
		                  (const DataTypes::LocationUpdate& request) const
		{
			switch (request.update_type_case())
			{
			case DataTypes::LocationUpdate::kInserted:
				context_->track_locations()->add(to_data_location(request.inserted()));
				return;
			case DataTypes::LocationUpdate::kDeleted:
				context_->track_locations()->remove(to_data_location(request.deleted()));
				return;
			case DataTypes::LocationUpdate::kUpdated:
				context_->track_locations()->update(to_data_location(request.updated()));
				return;
			case DataTypes::LocationUpdate::UPDATE_TYPE_NOT_SET: break;
			default: break;
			}
			throw std::exception("UpdateLocationRequestHandler update not implemented");
		}
	}
}
