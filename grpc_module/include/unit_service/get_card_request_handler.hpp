#ifndef GetCardRequestHandler_Included
#define GetCardRequestHandler_Included

#include <memory>
#include <include/grpc++/impl/codegen/completion_queue.h>
#include <services/unit_service.grpc.pb.h>
#include <request_handler.hpp>
#include <contracts/iunit_context.hpp>
#include <service_utils.hpp>
#include <contracts/devices/access_device/icommand_result.hpp>

using contracts::devices::IDeviceObserver;
using contracts::devices::access_device::ICommandResult;
namespace grpc_services
{
	namespace unit_service
	{		
		class GetCardRequestHandler : public RequestHandler<AsyncService>
			                          , public IDeviceObserver<ICommandResult>
		{
		public:
			GetCardRequestHandler(std::shared_ptr<AsyncService> service
				, grpc::ServerCompletionQueue* completion_queue
				, contracts::IUnitContextPtr context)
				: RequestHandler<AsyncService>(service, completion_queue)
				, responder_(&server_context_)
				, context_(context)
				, creation_time_(clock())
			{
				Proceed();
			}

			size_t get_hash_code() const override
			{
				auto h1 = std::hash<std::string>()(typeid(GetCardRequestHandler).name());
				auto h2 = std::hash<long>()(creation_time_);
				return h1 ^ (h2 << 1);
			}

			void CreateRequestHandler() override
			{
				new GetCardRequestHandler(service_, server_completion_queue_, context_);
			}

			void CreateRequest() override
			{
				service_->RequestGetCard(&server_context_, &request_
					, &responder_, server_completion_queue_
					, server_completion_queue_, this);
			}

			void ProcessRequest() override
			{
				//TODO refactor
				auto access_device_engine = context_->devices()->access_device_engine();
				//TODO check device valid
				auto device_name = request_.device_name();
				access_device_engine->add(device_name);
				access_device_engine->subscribe(this, device_name);

				access_device_engine->execute(device_name
					, contracts::devices::access_device::lights::lRedMain);
				
				std::cout << "Coordinator wants get card from location" << std::endl;
				/*
				// Finish only when something wrong
				responder_.Finish(response, grpc::Status::OK, this);
				*/
			}

			void on_error(const contracts::devices::DeviceException& exception) override
			{
				std::cout << exception.what() << std::endl;
			}

			void on_state(const contracts::devices::IDeviceState&    state) override
			{
			  //Not implemented
			}

			void 
			on_next(const ICommandResult& data) override
			{
				if (data.empty() || !data.ok())
					return; //TODO handle

				DataTypes::CardMsg response;
				response.set_card_num(data.to_string());

				auto access_device_engine = context_->devices()->access_device_engine();
				auto device_name = request_.device_name();
				access_device_engine->unsubscribe(this);
				access_device_engine->remove(device_name);

				responder_.Finish(response, grpc::Status::OK, this);
			}

		private:
			DataTypes::Device  request_;
			grpc::ServerAsyncResponseWriter<DataTypes::CardMsg> responder_;
			contracts::IUnitContextPtr context_;
			clock_t creation_time_;
		};
	}
}
#endif