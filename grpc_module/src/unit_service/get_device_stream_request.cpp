/*#include "unit_service/get_device_stream_request_handler.hpp"
#include <data/models/devices.hpp>

using namespace devices;
using namespace video_device;
namespace grpc_services
{
	namespace unit_service
	{
		bool update_device_if_changed( const Services::StreamMsg& request
		                              , data_model::DeviceId& device)
		{
			if (request.target_type_case() == Services::StreamMsg::kDevice)
			{
				if (request.device().device_type() != DataTypes::Capture)
					return false;
				device.set_name(request.device().device_name());
				return !device.is_empty();
			}
			return false;			
		}

		GetDeviceStreamRequestHandler::GetDeviceStreamRequestHandler
		(   Services::UnitService::AsyncService* service
			, grpc::ServerCompletionQueue*         completion_queue
			, contracts::IServiceContext*          context          )
			: RequestHandler<Services::UnitService::AsyncService>(service, completion_queue)
			, initialized_   (false)
			, correlation_id_(-1)
			, responder_(&server_context_)
			, context_  (context)
			, stopped_  (false)
			, request_created_(false)
			, device_id_ (std::make_unique<data_model::DeviceId>())		
		{		
			proceed();
		}

		GetDeviceStreamRequestHandler::~GetDeviceStreamRequestHandler()
		{
			if (engine_!= nullptr)
		  	engine_->unsubscribe(this);
		}

		void GetDeviceStreamRequestHandler::process_request()
		{ 	
			std::lock_guard<std::recursive_mutex> lock(mutex_);

			if (!try_resolve_dependencies())
			{
				complete();
				return;
			}
	
			auto canceled = server_context_.IsCancelled();
			stopped_      = request_.state() == DataTypes::Stopped;
			if (stopped_ || canceled)
			{
				if (canceled)
					logger_.info("Service context cancelled");
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
	

		void GetDeviceStreamRequestHandler::try_start_stream()
		{
			auto changed = update_device_if_changed(request_, *device_id_);
			if (changed)
			{
				logger_.info("Streaming started -> {0}", device_id_->name());
				engine_->add(*device_id_);
				engine_->subscribe(this, *device_id_);
			}
		}

		void GetDeviceStreamRequestHandler::complete()
		{			
			engine_->unsubscribe(this);
			engine_->remove(*device_id_);

			logger_.info("Streaming stopped -> {0}", device_id_->name());
			responder_.Finish(grpc::Status::OK, this);
			next();
		}

		void GetDeviceStreamRequestHandler::on_error(const DeviceException& exception)
		{}

		void GetDeviceStreamRequestHandler::on_state(const IDeviceState& state)
		{}

		void GetDeviceStreamRequestHandler::on_next(std::shared_ptr<IStreamData> data)
		{
			if (stopped_)
				return;

			auto compressed = data->jpeg_bytes();
			if (compressed.size() <= 0)
				return;
			DataTypes::FrameBytes bytes;			
			bytes.set_frame_data(std::string(compressed.begin(), compressed.end()));
			
			if (!can_process_ || server_context_.IsCancelled())
				return;

			std::lock_guard<std::recursive_mutex> lock(mutex_);
			responder_.Write(bytes, this);			
			can_process_ = server_context_.IsCancelled();
		}

		bool GetDeviceStreamRequestHandler::try_resolve_dependencies()
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

				return true;
			}
			catch (std::exception& ex)
			{
				logger_.error("GetDeviceStreamRequestHandler::try_resolve_dependencies {0}", ex.what());
				return false;
			}
		}
	}
}
*/