#include "unit_service/update_location_request_handler.hpp"

#include <helpers/request_adapters.hpp>
#include <data/models/location.hpp>

using namespace services_api::helpers;

namespace grpc_services
{
	namespace unit_service
	{
		void UpdateLocationRequestHandler::process_request()
		{
			google::protobuf::Empty response;

			for ( const auto& item : request_.items())
				update(item);			

			logger_.info("Coordinator wants update location");
			responder_.Finish(response, grpc::Status::OK, this);
		}

		void UpdateLocationRequestHandler::update
		                  (const DataTypes::LocationUpdate& request) const
		{
			switch (request.update_type_case())
			{
			case DataTypes::LocationUpdate::kInserted:
				context_->track_locations()->add(to_data_location(request.inserted()));
				break;
			case DataTypes::LocationUpdate::kDeleted:
				context_->track_locations()->remove(to_data_location(request.deleted()));
				break;
			case DataTypes::LocationUpdate::kUpdated:
				context_->track_locations()->update(to_data_location(request.updated()));
				break;
			case DataTypes::LocationUpdate::UPDATE_TYPE_NOT_SET: break;
			default: break;
			}
		}
	}
}
