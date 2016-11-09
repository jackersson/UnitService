#include <unit_service/open_door_request_handler.hpp>
#include <helpers/request_adapters.hpp>
namespace grpc_services
{
	namespace unit_service
	{
		void OpenDoorRequestHandler::ProcessRequest()
		{
			google::protobuf::Empty response;

			auto location = services_api::helpers::to_data_location(request_);
			context_->track_locations()->grant_access(location);

			logger_.info("Client wants open door");
			responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}
