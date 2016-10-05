#ifndef GetVideoStreamRequestHandler__INCLUDED
#define GetVideoStreamRequestHandler__INCLUDED
#include <memory>
#include <include/grpc++/impl/codegen/completion_queue.h>
#include <services/unit_service.grpc.pb.h>


namespace grpc_services
{
	namespace unit_service
	{
		class CreatePopulationHandler
		{
			enum RequestStatus { CREATE, PROCESS, CAN_WRITE, START_FINISH, FINISH };
		public:
			CreatePopulationHandler(std::shared_ptr<Services::UnitService::AsyncService> service
				, grpc::ServerCompletionQueue* completion_queue	)
				: service_(service)
				, server_completion_queue_(completion_queue)
				, responder_(&server_context_)
				, status_(CREATE)				
			{
				Proceed();
			}

			void Proceed()
			{
				if (status_ == CREATE)
				{
					status_ = PROCESS;
					service_->RequestGetVideoStream(&server_context_, &request_
						, &responder_, server_completion_queue_
						, server_completion_queue_, this);
				}
				else if (status_ == PROCESS)
				{
					new CreatePopulationHandler(service_, server_completion_queue_);
					processRequest();
				}
				else
				{
					//GPR_ASSERT(status_ == FINISH);
					delete this;
				}
			}

			void processRequest()
			{
				Services::FrameBytes response;
				//std::cout << "identification done size = " << result->matches().size() << std::endl;
				status_ = FINISH;
				responder_.Finish(grpc::Status::OK, this);
			}


		private:
			std::shared_ptr<Services::UnitService::AsyncService> service_;
			grpc::ServerCompletionQueue*                     server_completion_queue_;
			grpc::ServerContext                              server_context_;


			DataTypes::Location                        request_;
			grpc::ServerAsyncWriter<Services::FrameBytes>     responder_;

			RequestStatus status_;
		};
	}
}

#endif
