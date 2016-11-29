#ifndef UpdateLocationRequestHandler_Included
#define UpdateLocationRequestHandler_Included

#include <memory>
#include <helpers/request_handler.hpp>
#include <iservice_context.hpp>
#include <services/unit_service.grpc.pb.h>

namespace grpc_services
{
	namespace unit_service
	{
		class UpdateLocationRequestHandler 
			: public RequestHandler<Services::UnitService::AsyncService>
		{
		public:
			UpdateLocationRequestHandler(Services::UnitService::AsyncService* service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IServiceContext* context);

			void create_request_handler() override
			{
				new UpdateLocationRequestHandler(service_, server_completion_queue_, context_);
			}

			void create_request() override
			{
				service_->RequestUpdateLocations(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void process_request() override;

			static void create(Services::UnitService::AsyncService* service
				                , grpc::ServerCompletionQueue*        completion_queue
				                , contracts::IServiceContext*         context)
			{
				new UpdateLocationRequestHandler(service, completion_queue, context);
			}

		private:
			bool initialized_;

			void complete();
			bool try_resolve_dependencies();

			void update(const DataTypes::LocationUpdate&) const;			

			DataTypes::LocationUpdates  request_;
			grpc::ServerAsyncResponseWriter<google::protobuf::Empty> responder_;
			contracts::IServiceContext* context_;
		};
	}
}
#endif