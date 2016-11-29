#include "unit_service/enroll_request_handler.hpp"

namespace grpc_services
{
	namespace unit_service
	{
		EnrollRequestHandler::EnrollRequestHandler
	  (   Services::UnitService::AsyncService* service
			, grpc::ServerCompletionQueue* completion_queue
			, contracts::IServiceContext* context)
			: RequestHandler<Services::UnitService::AsyncService>(service, completion_queue)
			, responder_(&server_context_)
			, context_(context)
			, initialized_(false)
		{
			proceed();
		}

		void EnrollRequestHandler::process_request()
		{
			DataTypes::Faces response;
			//TODO implement
			responder_.Finish(response, grpc::Status::OK, this);
		}
	}
}
