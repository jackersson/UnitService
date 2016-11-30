#ifndef GetStreamRequestHandler_Included
#define GetStreamRequestHandler_Included

#include <helpers/request_handler.hpp>
#include <iservice_context.hpp>
#include <services/unit_service.grpc.pb.h>
#include <atomic>

namespace grpc_services
{
	namespace unit_service
	{	
		class GetStreamRequestHandler
			: public RequestHandler<Services::UnitService::AsyncService>
			, public devices::IDeviceObserver<video_device::IStreamDataPtr>
		{
		public:
			GetStreamRequestHandler
			( Services::UnitService::AsyncService* service
			, grpc::ServerCompletionQueue*         completion_queue
			, contracts::IServiceContext*          context);

			void create_request_handler() override
			{
				if (!request_created_)
				{
					new GetStreamRequestHandler(service_, server_completion_queue_, context_);
					request_created_ = true;
				}
			}

			void create_request() override
			{
				service_->RequestGetStream(&server_context_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void process_request() override;

			static void create(Services::UnitService::AsyncService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IServiceContext*     context)
			{
				new GetStreamRequestHandler(service, completion_queue, context);
			}

			void on_error(const devices::DeviceException& exception) override;
			void on_state(const devices::IDeviceState&    state    ) override;
			void on_next (video_device::IStreamDataPtr     data     ) override;

		private:

			bool try_resolve_dependencies();
			bool try_start_stream(bool is_location);
			void complete();

			bool try_start_location_stream();
			bool try_start_device_stream  (const data_model::DeviceId&);

			bool is_changed(bool& is_location) const;

			std::unique_ptr<data_model::DeviceId> device_   ;
			std::unique_ptr<data_model::Location> location_ ;

			GetStreamRequestHandler(const GetStreamRequestHandler& other) = delete;
			GetStreamRequestHandler& operator=(const GetStreamRequestHandler&) = delete;

			mutable std::recursive_mutex mutex_;

			std::atomic_bool can_process_;

			bool initialized_    ;
			bool request_created_;
			bool stopped_        ;

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