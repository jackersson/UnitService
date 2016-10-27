#include <unit_service/enroll_request_handler.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		void EnrollRequestHandler::ProcessRequest()
		{
			DataTypes::Faces response;
			//TODO implement
			responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}
