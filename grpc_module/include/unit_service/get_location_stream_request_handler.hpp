#ifndef GetLocationStreamRequestHandler_Included
#define GetLocationStreamRequestHandler_Included

#include <helpers/request_handler.hpp>
#include <contracts/iservice_context.hpp>
#include <services/unit_service.grpc.pb.h>

namespace grpc_services
{
	namespace unit_service
	{	
		class GetLocationStreamRequestHandler
			: public RequestHandler<Services::UnitService::AsyncService>
			, public
			contracts::devices::IDeviceObserver<contracts::devices::video_device::IStreamData>
		{
		public:
			GetLocationStreamRequestHandler
			(Services::UnitService::AsyncService* service
				, grpc::ServerCompletionQueue*         completion_queue
				, contracts::IServiceContext*          context);

			void create_request_handler() override
			{
				if (!initialized_)
				{
					new GetLocationStreamRequestHandler(service_, server_completion_queue_, context_);
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

			static void create(Services::UnitService::AsyncService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IServiceContext*     context)
			{
				new GetLocationStreamRequestHandler(service, completion_queue, context);
			}

			void on_error(const contracts::devices::DeviceException& exception) override;
			void on_state(const contracts::devices::IDeviceState&    state) override;
			void on_next(const contracts::devices::video_device::IStreamData& data) override;

		private:

			bool try_start_stream();
			void complete();

			std::unique_ptr<data_model::Location> location_;

			GetLocationStreamRequestHandler(const GetLocationStreamRequestHandler& other) = delete;
			GetLocationStreamRequestHandler& operator=(const GetLocationStreamRequestHandler&) = delete;

			mutable std::recursive_mutex mutex_;

			bool can_process_;
			bool initialized_;
			bool stopped_;

			google::protobuf::int64 correlation_id_;

			Services::StreamMsg  request_;
			grpc::ServerAsyncReaderWriter<DataTypes::FrameBytes, Services::StreamMsg > responder_;
			contracts::IServiceContext* context_;

			contracts::locations::ITrackLocationsEngine* track_locations_;
			contracts::devices::video_device::IVideoEngine* engine_;
		};
	}
}
#endif