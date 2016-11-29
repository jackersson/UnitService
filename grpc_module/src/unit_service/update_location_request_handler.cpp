#include "unit_service/update_location_request_handler.hpp"

#include <helpers/request_adapters.hpp>
#include <data/models/location.hpp>
#include <future>

using namespace services_api::helpers;

namespace grpc_services
{
	namespace unit_service
	{
		UpdateLocationRequestHandler::UpdateLocationRequestHandler(Services::UnitService::AsyncService* service
			, grpc::ServerCompletionQueue* completion_queue
			, contracts::IServiceContext* context)
			: RequestHandler<Services::UnitService::AsyncService>(service, completion_queue)
			, initialized_(false)
			, responder_  (&server_context_)
			, context_    (context)
		{
			proceed();
		}

		void UpdateLocationRequestHandler::process_request()
		{
			logger_.info("Update location -> in");
			if  (!try_resolve_dependencies())
			{
				complete();
				return;
			}			

			std::async(std::launch::async, [this]() {
				try
				{
					for (const auto& item : request_.items())
						update(item);
				}
				catch (std::exception& ex)
				{
					logger_.error(ex.what());
				}
			});
			complete();
		}

		void UpdateLocationRequestHandler::complete()
		{
			logger_.info("Update location -> out ok");
			google::protobuf::Empty response;

			next();
			responder_.Finish(response, grpc::Status::OK, this);
		}

		void UpdateLocationRequestHandler::update
		                  (const DataTypes::LocationUpdate& request) const
		{
			switch (request.update_type_case())
			{
			case DataTypes::LocationUpdate::kInserted:
				context_->track_locations()->add(to_data_location(request.inserted()));
				return;
			case DataTypes::LocationUpdate::kDeleted:
				context_->track_locations()->remove(to_data_location(request.deleted()));
				return;
			case DataTypes::LocationUpdate::kUpdated:
				context_->track_locations()->update(to_data_location(request.updated()));
				return;
			case DataTypes::LocationUpdate::UPDATE_TYPE_NOT_SET: break;
			default: break;
			}
			throw std::exception("UpdateLocationRequestHandler update not implemented");
		}

		bool UpdateLocationRequestHandler::try_resolve_dependencies()
		{
			if (initialized_)
				return initialized_;

			try
			{
				initialized_ = true;

				if (context_ == nullptr)
					throw std::exception("Context can't be null");
				auto tracking = context_->track_locations();
				if (tracking == nullptr)
					throw std::exception("Devices can't be null");

				return true;
			}
			catch (std::exception& ex)
			{
				logger_.error("UpdateLocationRequestHandler::try_resolve_dependencies {0}", ex.what());
				return false;
			}
		}
	}
}
