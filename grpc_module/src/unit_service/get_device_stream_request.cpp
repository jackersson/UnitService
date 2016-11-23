#include "unit_service/get_device_stream_request_handler.hpp"
#include <boost/thread/future.hpp>
#include <data/models/devices.hpp>

using namespace contracts::devices;
using namespace video_device;
namespace grpc_services
{
	namespace unit_service
	{
		bool update__device_if_changed( const Services::StreamMsg& request
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
			, grpc::ServerCompletionQueue* completion_queue
			, contracts::IServiceContext* context)
			: RequestHandler<Services::UnitService::AsyncService>(service, completion_queue)
			, responder_(&server_context_)
			, context_(context)
			, initialized_   (false)
			, read_requested_(false)
			, processed_     (false)
			, correlation_id_(-1)

		{
			if (context_ == nullptr)
				throw std::exception("Context can't be null");
			auto devices = context_->devices();
			if (devices == nullptr)
				throw std::exception("Devices can't be null");

			engine_ = devices->directshow_device_engine();
			if (engine_ == nullptr)
				throw std::exception("DirectShow Device engine can't be null");
		
			device_id_ = std::make_unique<data_model::DeviceId>();
			
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
			can_process_ = true;
			std::cout << "can_process_ " << can_process_ << std::endl;
			if (processed_)
				return;
			processed_ = true;
			logger_.info("Streaming session starts");
			boost::async([this]()
			{					
				while ( request_.state() != DataTypes::Stopped
					   && !server_context_.IsCancelled())
				{
					read_requested_ = false;
					auto corr_id = request_.correlation_id();
					if (correlation_id_ == corr_id)
					{
						std::this_thread::sleep_for(std::chrono::seconds(1));
						continue;
					}
					correlation_id_ = corr_id;

					auto changed = update__device_if_changed(request_, *device_id_);
					if (changed)
					{
						logger_.info("Streaming started -> {0}", device_id_->name());
						engine_->add(*device_id_);
						engine_->subscribe(this, *device_id_);
					}
					std::lock_guard<std::recursive_mutex> lock(mutex_);
					read_requested_ = true;				
					responder_.Read(&request_, this);					
				} 
			}).then([this](boost::future<void> f)
			{
				if (server_context_.IsCancelled())
					std::cout << "server_context_ canceled" << std::endl;
				engine_->unsubscribe(this);
				//engine_->remove(*device_id_);

				logger_.info("Streaming stopped -> {0}", device_id_->name());
				next();
				responder_.Finish(grpc::Status::OK, this);
			});				

		}			

		void GetDeviceStreamRequestHandler::on_error(const DeviceException& exception)
		{}

		void GetDeviceStreamRequestHandler::on_state(const IDeviceState& state)
		{}

		void GetDeviceStreamRequestHandler::on_next(const IStreamData& data)
		{
			if (read_requested_ || request_.state() == DataTypes::Stopped)
				return;

			auto compressed = data.jpeg_bytes();
			if (compressed.size() <= 0)
				return;
			DataTypes::FrameBytes bytes;			
			bytes.set_frame_data(std::string(compressed.begin(), compressed.end()));
			
			if (!can_process_ || server_context_.IsCancelled())
				return;

			std::lock_guard<std::recursive_mutex> lock(mutex_);
			responder_.Write(bytes, this);			
			can_process_ = server_context_.IsCancelled();
			std::cout << "can_process_ " << can_process_ << std::endl;

		}
	}
}