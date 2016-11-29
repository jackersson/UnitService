#include "unit_service/get_location_stream_request_handler.hpp"
#include <data/models/devices.hpp>
#include <data/models/location.hpp>
#include <helpers/request_adapters.hpp>
#include <uuid/uuid_io.hpp>
#include <thread_pool.hpp>

using namespace services_api::helpers;
using namespace devices;

namespace grpc_services
{
	namespace unit_service
	{
		bool update_location_if_changed( const Services::StreamMsg& request
			                             , data_model::Location& location)
		{
			if (request.target_type_case() == Services::StreamMsg::kLocation)
			{			
				location.set_id(to_data_model_key(request.location().id()));
				return !location.id().is_empty();
			}
			return false;
		}

		GetLocationStreamRequestHandler::GetLocationStreamRequestHandler
		(Services::UnitService::AsyncService* service
			, grpc::ServerCompletionQueue* completion_queue
			, contracts::IServiceContext* context)
			: RequestHandler<Services::UnitService::AsyncService>(service, completion_queue)
			, initialized_(false)
			, correlation_id_(-1)
			, responder_(&server_context_)
			, context_(context)
			, stopped_(false)
			, request_created_(false)
			, busy_(false)
			, location_(std::make_unique<data_model::Location>())
		{			
			proceed();
		}
		
		void GetLocationStreamRequestHandler::process_request()
		{
			//std::lock_guard<std::recursive_mutex> lock(mutex_);

			if (!try_resolve_dependencies())
			{
				complete();
				return;
			}

			auto canceled = server_context_.IsCancelled();
			stopped_ = request_.state() == DataTypes::Stopped;

			if (stopped_ || canceled)
			{
				if (canceled)
					logger_.info("Service context cancelled");
				else 
					logger_.info("Streaming cancelled by user");
				complete();
				return;
			}

			can_process_ = true;
			auto corr_id = request_.correlation_id();
			if (correlation_id_ == corr_id)
				return;

			correlation_id_ = corr_id;

			try_start_stream();		

			responder_.Read(&request_, this);
		}


		bool GetLocationStreamRequestHandler::try_start_stream()
		{
			auto changed = update_location_if_changed(request_, *location_);
			if (changed)
			{
				logger_.info("Streaming requested -> Location {0}"
					, to_string(location_->id().guid()));

				data_model::Location actual;
				auto ok = track_locations_->try_get(location_->id(), actual);

				if (ok)
				{
					engine_->subscribe(this, actual.capture_device());
					logger_.info( "Streaming started -> Location {0}"
						          , to_string(location_->id().guid()));
					return true;
				}
				logger_.info( "Streaming failed -> Location {0} not exists"
					          , to_string(location_->id().guid()));
				return false;
			}
			return false;
		}

		void GetLocationStreamRequestHandler::complete()
		{		
			engine_->unsubscribe(this);

			logger_.info( "Streaming stopped -> {0}"
				           , to_string(location_->id().guid()));
			responder_.Finish(grpc::Status::OK, this);
			next();
		}

		void GetLocationStreamRequestHandler::on_error(const DeviceException& exception)
		{}

		void GetLocationStreamRequestHandler::on_state(const IDeviceState& state)
		{}

		void GetLocationStreamRequestHandler::on_next(std::shared_ptr<video_device::IStreamData> data)
		{
			if (stopped_|| busy_)
				return;
			busy_ = true;
		//	utils::threading::submit_job_nowait([data, this]()
		//	{
			  auto compressed = data->jpeg_bytes();
				if (compressed.size() <= 0)
				{
					busy_ = false;
					return;
				}
			  DataTypes::FrameBytes bytes;
			  bytes.set_frame_data(std::string(compressed.begin(), compressed.end()));
			  
				if (server_context_.IsCancelled())
				{
					process_request();
					busy_ = false;
					return;
				}

				if (!can_process_)
				{
					busy_ = false;
					return;
				}

			  can_process_ = false;
			  responder_.Write(bytes, this);

				busy_ = false;

			//});
			
		}

		bool GetLocationStreamRequestHandler::try_resolve_dependencies()
		{
			if (initialized_)
				return initialized_;

			try
			{
				initialized_ = true;

				if (context_ == nullptr)
					throw std::exception("Context can't be null");

				auto devices = context_->devices();
				if (devices == nullptr)
					throw std::exception("Devices can't be null");

				engine_ = devices->directshow_device_engine();
				if (engine_ == nullptr)
					throw std::exception("DirectShow Device engine can't be null");

				track_locations_ = context_->track_locations();
				if (track_locations_ == nullptr)
					throw std::exception("Track locations engine can't be null");

				return true;
			}
			catch (std::exception& ex)
			{
				logger_.error("GetLocationStreamRequestHandler::try_resolve_dependencies {0}", ex.what());
				return false;
			}
		}
	}
}
