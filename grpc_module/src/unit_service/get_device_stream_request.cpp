#include "unit_service/get_device_stream_request_handler.hpp"

namespace grpc_services
{
	namespace unit_service
	{
		void GetDeviceStreamRequestHandler::process_request()
		{
			google::protobuf::Empty response;
			//TODO handle frames
			//context_->track_locations()->grant_access(request_);

			std::cout << "Client wants capture device video stream" << std::endl;
			//responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}