#ifndef GetDeviceStreamRequestHandler_Included
#define GetDeviceStreamRequestHandler_Included

#include <helpers/request_handler.hpp>
#include <contracts/iservice_context.hpp>
#include <services/unit_service.grpc.pb.h>

namespace grpc_services
{
	namespace unit_service
	{	
		class GetDeviceStreamRequestHandler 
			: public RequestHandler<Services::UnitService::AsyncService>
			, public 
			contracts::devices::IDeviceObserver<contracts::devices::video_device::IStreamData>
		{
		public:
			GetDeviceStreamRequestHandler
			 (  Services::UnitService::AsyncService* service
				, grpc::ServerCompletionQueue*         completion_queue
				, contracts::IServiceContext*          context         );

			~GetDeviceStreamRequestHandler();
			

			void create_request_handler() override
			{
				if (!initialized_)
				{
					new GetDeviceStreamRequestHandler(service_, server_completion_queue_, context_);
					initialized_ = true;
				}
			}

			void create_request() override
			{
				service_->RequestGetDeviceStream(&server_context_
					             , &responder_, server_completion_queue_
					             , server_completion_queue_, this);
			}

			void process_request() override;

			static void create( Services::UnitService::AsyncService* service
				                , grpc::ServerCompletionQueue* completion_queue
				                , contracts::IServiceContext*     context)
			{
				new GetDeviceStreamRequestHandler(service, completion_queue, context);
			}

			void on_error(const contracts::devices::DeviceException& exception) override;
			void on_state(const contracts::devices::IDeviceState&    state) override;
			void on_next (const contracts::devices::video_device::IStreamData& data) override;
			
		private:
			std::unique_ptr<data_model::DeviceId> device_id_;

			GetDeviceStreamRequestHandler(const GetDeviceStreamRequestHandler& other) = delete;
			GetDeviceStreamRequestHandler& operator=(const GetDeviceStreamRequestHandler&) = delete;

			mutable std::recursive_mutex mutex_;
			bool can_process_;
			bool processed_     ;
			bool initialized_   ;
			bool read_requested_;
			google::protobuf::int64 correlation_id_;
			Services::StreamMsg  request_;
			grpc::ServerAsyncReaderWriter<DataTypes::FrameBytes, Services::StreamMsg > responder_;
			contracts::IServiceContext* context_;

			contracts::devices::video_device::IVideoEngine* engine_;
		};
	}
}
#endif