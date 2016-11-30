#include "unit_service/get_stream_request_handler.hpp"
#include <data/models/devices.hpp>
#include <data/models/location.hpp>
#include <helpers/request_adapters.hpp>
#include <uuid/uuid_io.hpp>

using namespace services_api::helpers;
using namespace devices;

namespace grpc_services
{
	namespace unit_service
	{
		bool GetStreamRequestHandler::is_changed( bool& is_location) const
		{			
			switch (request_.target_type_case())
			{
			case Services::StreamMsg::kDevice:			
				device_->set_name(request_.device().device_name());
				is_location = false;
				return !device_->is_empty();
			case Services::StreamMsg::kLocation: 
				location_->set_id(to_data_model_key(request_.location().id()));
				is_location = true;
				return !location_->id().is_empty();
			case Services::StreamMsg::TARGET_TYPE_NOT_SET:
			default: 
				return false;
			}
		}

		GetStreamRequestHandler::GetStreamRequestHandler
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
			, location_(std::make_unique<data_model::Location>())
			, device_  (std::make_unique<data_model::DeviceId>())
		{			

			proceed();
		}
		
		void GetStreamRequestHandler::process_request()
		{			
			if (!try_resolve_dependencies())
			{
				complete();
				return;
			}

			if  (  request_.state() == DataTypes::Stopped 
				  || server_context_.IsCancelled())
			{
				if (server_context_.IsCancelled())
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

			auto is_location = false;
			if (is_changed(is_location))
			{
				if (!try_start_stream(is_location))
				{
					complete();
					return;
				}
			}

			responder_.Read(&request_, this);
		}


		bool GetStreamRequestHandler::try_start_stream(bool is_location)
		{			
			if (is_location)
				return try_start_location_stream();
		  return try_start_device_stream(*device_);
		}

		bool GetStreamRequestHandler::try_start_location_stream()
		{
			logger_.info("Streaming requested -> Location {0}"
			           	, to_string(location_->id().guid()));

			data_model::Location actual;
			auto found = track_locations_->try_get(location_->id(), actual);

			if (found)
				return try_start_device_stream(actual.capture_device());

		  logger_.info( "Streaming failed -> Location {0} not exists"
				           , to_string(location_->id().guid()));
			return false;
		}

		bool GetStreamRequestHandler::try_start_device_stream
		      (const data_model::DeviceId& device)
		{					
			if (!engine_->enumerator().connected(device))
			{
				logger_.info("Streaming failed -> Device {0} not connected", device.name());
				return false;
			}

			logger_.info("Streaming started -> Device {0}", device.name());
			engine_->add(device);
			engine_->subscribe(this, device);
			return true;
		}

		void GetStreamRequestHandler::complete()
		{		
			engine_->unsubscribe(this);
			engine_->remove(*device_);

			if (!location_->id().is_empty())
			  logger_.info( "Streaming stopped -> {0}"
				             , location_->id().to_string());
			else if (!device_->is_empty() )
				logger_.info("Streaming stopped -> {0}", device_->name());
			else 
				logger_.info("Streaming stopped");

			next();
			responder_.Finish(grpc::Status::OK, this);
		}

		void GetStreamRequestHandler::on_error(const DeviceException& exception)
		{}

		void GetStreamRequestHandler::on_state(const IDeviceState& state)
		{}

		void GetStreamRequestHandler::on_next(video_device::IStreamDataPtr data)
		{
			if (stopped_)
				return;		
		
			DataTypes::FrameBytes bytes;
			if (data != nullptr)
			{
				auto compressed = data->jpeg_bytes();
				if (compressed.size() > 0)
					bytes.set_frame_data(std::string(compressed.begin(), compressed.end()));
			}

			if (server_context_.IsCancelled())				
				return;

			if (!can_process_)
			{
				process_request();
				return;
			}

			can_process_ = false;
			responder_.Write(bytes, this);		
		}

		bool GetStreamRequestHandler::try_resolve_dependencies()
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
