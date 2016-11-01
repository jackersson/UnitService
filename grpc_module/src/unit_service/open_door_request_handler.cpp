#include <unit_service/open_door_request_handler.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		void OpenDoorRequestHandler::ProcessRequest()
		{
			google::protobuf::Empty response;

			context_->track_locations()->grant_access(request_);

			logger_.info("Client wants open door");
			responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}