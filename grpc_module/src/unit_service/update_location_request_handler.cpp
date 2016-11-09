#include <unit_service/update_location_request_handler.hpp>
#include <helpers/request_adapters.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		void UpdateLocationRequestHandler::ProcessRequest()
		{
			google::protobuf::Empty response;

			auto location = services_api::helpers::to_data_location(request_);
			context_->track_locations()->update(location);

			logger_.info("Coordinator wants update location");
			responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}
