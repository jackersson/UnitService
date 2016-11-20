#include <unit_service/open_door_request_handler.hpp>
#include <helpers/request_adapters.hpp>
#include <data/models/location.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		void OpenDoorRequestHandler::process_request()
		{
			google::protobuf::Empty response;

			logger_.info("Open door request -> in {}", request_.id().guid());
			
			auto location = services_api::helpers::to_data_location(request_);

			auto result = context_->track_locations()->grant_access(location);

			logger_.info("Open door request -> out {0}", result);
			next();
			responder_.Finish(response, grpc::Status::OK, this);
		}		
	}
}
