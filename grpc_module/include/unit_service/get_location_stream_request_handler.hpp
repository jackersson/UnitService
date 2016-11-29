#ifndef GetLocationStreamRequestHandler_Included
#define GetLocationStreamRequestHandler_Included

#include <helpers/request_handler.hpp>
#include <iservice_context.hpp>
#include <services/unit_service.grpc.pb.h>

namespace grpc_services
{
	namespace unit_service
	{	
		class GetLocationStreamRequestHandler
			: public RequestHandler<Services::UnitService::AsyncService>
			, public devices::IDeviceObserver<video_device::IStreamDataPtr>
		{
		public:
			GetLocationStreamRequestHandler
			(Services::UnitService::AsyncService* service
				, grpc::ServerCompletionQueue*         completion_queue
				, contracts::IServiceContext*          context);

			void create_request_handler() override
			{
				if (!request_created_)
				{
					new GetLocationStreamRequestHandler(service_, server_completion_queue_, context_);
					request_created_ = true;
				}
			}

			void create_request() override
			{
				service_->RequestGetLocationStream(&server_context_
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

			void on_error(const devices::DeviceException& exception) override;
			void on_state(const devices::IDeviceState&    state    ) override;
			void on_next(video_device::IStreamDataPtr     data     ) override;

		private:

			bool try_resolve_dependencies();
			bool try_start_stream();
			void complete();


			std::unique_ptr<data_model::Location> location_;

			GetLocationStreamRequestHandler(const GetLocationStreamRequestHandler& other) = delete;
			GetLocationStreamRequestHandler& operator=(const GetLocationStreamRequestHandler&) = delete;

			mutable std::recursive_mutex mutex_;

			std::atomic_bool can_process_;

			bool initialized_    ;
			//bool can_process_    ;
			bool request_created_;
			bool stopped_        ;
			bool busy_           ;

			google::protobuf::int64 correlation_id_;

			Services::StreamMsg  request_;
			grpc::ServerAsyncReaderWriter<DataTypes::FrameBytes, Services::StreamMsg > responder_;
			contracts::IServiceContext* context_;

			contracts::locations::ITrackLocationsEngine* track_locations_;
			video_device::IVideoEngine* engine_;
		};
	}
}
#endif