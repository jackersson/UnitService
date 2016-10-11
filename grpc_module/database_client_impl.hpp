#ifndef DatabaseClientImpl_INCLUDED
#define DatabaseClientImpl_INCLUDED

#include <grpc++/grpc++.h>
#include <contracts/services/iservice.hpp>
#include <services/database_service.grpc.pb.h>
#include "include/client_context.hpp"
#include <src/cpp/server/dynamic_thread_pool.h>
#include "include/database_service/database_client_calls.hpp"
#include <contracts/services/idatabase_api.hpp>

using grpc::ServerBuilder;

namespace grpc_services
{
	typedef std::function<void()> RpcCallbackFunction;

	typedef std::pair<std::shared_ptr<grpc::CompletionQueue>
		, RpcCallbackFunction> CallHandler;
	typedef std::map<std::string, CallHandler> CallHandlers;

	class DatabaseClientImpl : public contracts::services::IService
		                       , public contracts::services::IDatabaseApi	                    
	{
	public:
		explicit DatabaseClientImpl(const ClientContext& context) 
			: active_(false), context_(context)
		{
			DatabaseClientImpl::init();
		}

		~DatabaseClientImpl()
		{
			DatabaseClientImpl::stop();
		}

		void init() override
		{
			channel_ = grpc::CreateChannel(context_.Address().FormattedAddress()
				, grpc::InsecureChannelCredentials());
			thread_pool_ = std::make_shared<grpc::DynamicThreadPool>(MAX_THREAD_POOL_CAPACITY);
			stub_ = Services::DatabaseService::NewStub(channel_);

			add_call_handler<AsyncGetRequestCall>   ();
			add_call_handler<AsyncCommitRequestCall>();
		}

		void start() override
		{
			if (active_)
				return;
			
			active_ = true;

			for (auto it = handlers_.begin(); it != handlers_.end(); ++it)
				thread_pool_->Add(it->second.second);

			std::cout << "Client connected to " << context_.Address().FormattedAddress() << std::endl;
		}

		void stop() override
		{			
			for (auto it : handlers_)
				it.second.first->Shutdown();
			handlers_.clear();
		}

		std::shared_ptr<Services::GetResponse> 
			get( const Services::GetRequest& request ) override
		{			
			DataTypes::MessageBytes message;
			std::string bytes;
			request.SerializeToString(&bytes);
			message.set_data(bytes);
			message.set_type(DataTypes::DataType::GetRequestType);

			auto it = handlers_.find(typeid(AsyncGetRequestCall).name());
			if (it == handlers_.end())
				return nullptr;

			auto call = new AsyncGetRequestCall;
			call->reader = stub_->AsyncGet(&call->context, message, it->second.first.get());
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));		

			auto future = call->promise_.get_future();
			std::chrono::milliseconds span(200);
			future.wait_for(span);
			return future.get();
		}

		//TODO make as get()
		std::shared_ptr<Services::CommitResponse>
			commit( const Services::CommitRequest& request	) override
		{
			DataTypes::MessageBytes message;
			std::string bytes;
			request.SerializeToString(&bytes);
			message.set_data(bytes);
			message.set_type(DataTypes::DataType::CommitRequestType);

			auto it = handlers_.find(typeid(AsyncGetRequestCall).name());
			if (it == handlers_.end())
				return nullptr;

			auto call = new AsyncCommitRequestCall;
			call->reader = stub_->AsyncGet(&call->context, message, it->second.first.get());
			call->reader->Finish(&call->response, &call->status, reinterpret_cast<void*>(call));
		
			return nullptr;
		}


	private:
		template<typename T>
		void add_call_handler()
		{						
			auto cq = std::make_shared<grpc::CompletionQueue>();

			auto callback = std::bind( &DatabaseClientImpl::async_complete_rpc<T>
				                        , this, cq.get());
			handlers_.insert(
				std::pair<std::string, CallHandler>(
					typeid(T).name(), CallHandler(cq, callback)));
		}

		template <typename T>
		void async_complete_rpc(grpc::CompletionQueue* queue) const
		{
			void* got_tag;
			auto  ok = false;
			
			while (queue->Next(&got_tag, &ok))
			{			
				auto call = static_cast<T*>(got_tag);
				try
				{
					if (call->status.ok())
					{
						call->parsed_response();
						std::cout << "Received" << std::endl;
					}
					else
						std::cout << "Rpc failed" << std::endl;
				}
				catch (std::exception& ex){
					std::cout << ex.what() << std::endl;
				}
				delete call;
			}
		}

		bool active_;
		std::shared_ptr<grpc::ThreadPoolInterface>       thread_pool_;
		std::unique_ptr<Services::DatabaseService::Stub> stub_       ;
		std::shared_ptr<grpc::Channel>                   channel_    ;

		ClientContext context_;
		CallHandlers handlers_;

		const int MAX_THREAD_POOL_CAPACITY = 10;

		DatabaseClientImpl(const DatabaseClientImpl&) = delete;
		DatabaseClientImpl& operator=(const DatabaseClientImpl&) = delete;
	};
}

#endif