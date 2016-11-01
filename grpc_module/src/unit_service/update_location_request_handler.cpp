#include <unit_service/update_location_request_handler.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		void UpdateLocationRequestHandler::ProcessRequest()
		{
			google::protobuf::Empty response;

			//TODO update location in local storage and coordinator
			context_->repository()->get<DataTypes::Location>()->local()->update(&request_);

			logger_.info("Coordinator wants update location");
			responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}