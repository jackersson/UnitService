#include <unit_service/open_door_request_handler.hpp>
#include <helpers/request_adapters.hpp>
#include <data/models/location.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		OpenDoorRequestHandler::OpenDoorRequestHandler
		  ( Services::UnitService::AsyncService* service
			, grpc::ServerCompletionQueue* completion_queue
			, contracts::IServiceContext* context)
			: RequestHandler<Services::UnitService::AsyncService>(service, completion_queue)
			, responder_(&server_context_)
			, context_(context)
			, initialized_(false)
		{			
			proceed();
		}

		void OpenDoorRequestHandler::process_request()
		{
			logger_.info("Open door request -> in {}", request_.id().guid());

			if ( !try_resolve_dependencies())
			{
				complete(false);
				return;
			}
			
			auto location = services_api::helpers::to_data_location(request_);
			complete(context_->track_locations()->grant_access(location));
		}	

		void OpenDoorRequestHandler::complete(bool result)
		{
			logger_.info("Open door request -> out {0}", result);

			google::protobuf::Empty response;
			responder_.Finish(response, grpc::Status::OK, this);
			next();
		}

		bool OpenDoorRequestHandler::try_resolve_dependencies()
		{
			if (initialized_)
				return initialized_;

			try
			{
				initialized_ = true;

				if (context_ == nullptr)
					throw std::exception("Context can't be null");
				if (context_->track_locations() == nullptr)
					throw std::exception("Track locations can't be null");

				return true;
			}
			catch (std::exception& ex)
			{
				logger_.error("OpenDoorRequestHandler::try_resolve_dependencies {0}", ex.what());
				return false;
			}
		}
	}
}
