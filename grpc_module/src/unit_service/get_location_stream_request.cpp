#include "unit_service/get_location_stream_request_handler.hpp"

namespace grpc_services
{
	namespace unit_service
	{
		void GetLocationStreamRequestHandler::process_request()
		{
			google::protobuf::Empty response;
			//TODO handle frames
			//context_->track_locations()->grant_access(request_);

			std::cout << "Client wants capture location video stream" << std::endl;
			//responder_.Finish(response, grpc::Status::OK, this);
		}

	}
}