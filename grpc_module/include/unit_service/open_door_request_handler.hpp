#ifndef OpenDoorRequestHandler_Included
#define OpenDoorRequestHandler_Included

#include <memory>
#include <include/grpc++/impl/codegen/completion_queue.h>
#include <services/unit_service.grpc.pb.h>

namespace grpc_services
{
	namespace unit_service
	{
		typedef Services::UnitService::AsyncService UnitServicePtr;

		class OpenDoorRequestHandler
		{
			enum RequestStatus { CREATE, PROCESS, FINISH };
		public:

			OpenDoorRequestHandler(std::shared_ptr<UnitServicePtr> service
				, grpc::ServerCompletionQueue* completion_queue)
				: service_(service)
				, server_completion_queue_(completion_queue)
				, responder_(&server_context_)
				, status_(CREATE)
			{
				Proceed();
			}

		//	virtual ~OpenDoorRequestHandler() {	}
			/*
			void CreateRequestHandler()
			{
				new OpenDoorRequestHandler(service_, server_completion_queue_);
			}
			void CreateRequest()
			{
				std::cout << "here" << std::endl;
				service_->RequestOpenDoor(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}
			void ProcessRequest()
			{
				google::protobuf::Empty empty;

				responder_.Finish(empty, grpc::Status::OK, this);
			}
			*/
			void Proceed()
			{
				if (status_ == CREATE)
				{
					status_ = PROCESS;
					service_->RequestOpenDoor(&server_context_, &request_
						, &responder_, server_completion_queue_
						, server_completion_queue_, this);
				}
				else if (status_ == PROCESS)
				{
					new OpenDoorRequestHandler(service_, server_completion_queue_);
					TryProcessRequest();
				}
				else
				{
					//GPR_ASSERT(status_ == FINISH);
					delete this;
				}
			}



			void TryCreateRequest()
			{
				try
				{
					//CreateRequest();
				}
				catch (std::exception& ex)
				{
					//TODO log
					std::cout << ex.what() << std::endl;
				}
			}

			void TryProcessRequest()
			{
				google::protobuf::Empty response;

				status_ = FINISH;
				responder_.Finish(response, grpc::Status::OK, this);
			}

			void Next()
			{
				switch (status_)
				{
				case CREATE:
					status_ = PROCESS;
					break;
				case PROCESS:
					status_ = FINISH;
					break;
				default: break;
				}
			}

		protected:
			std::shared_ptr<UnitServicePtr> service_;
			grpc::ServerCompletionQueue* server_completion_queue_;
			grpc::ServerContext          server_context_;

		private:
			OpenDoorRequestHandler(const OpenDoorRequestHandler&) = delete;
			OpenDoorRequestHandler& operator=(const OpenDoorRequestHandler&) = delete;

			RequestStatus status_;

			DataTypes::Location  request_;
			grpc::ServerAsyncResponseWriter<google::protobuf::Empty>    responder_;
		};
	}
}
#endif