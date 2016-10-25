#ifndef ServiceUtils_INCLUDED
#define ServiceUtils_INCLUDED

#include <datatypes/queries.pb.h>
#include <datatypes/commands.pb.h>
#include <include/grpc++/impl/codegen/completion_queue.h>
#include <future>

#include <grpc++/grpc++.h>
#include <services/unit_service.grpc.pb.h>

namespace grpc_services
{
	typedef Services::UnitService::AsyncService AsyncService;
	
	typedef std::function<void()> RpcCallbackFunction;

	//typedef std::pair<std::shared_ptr<grpc::CompletionQueue>
		//, RpcCallbackFunction> CallHandler;
//

	template<typename TQueue>
	struct RequestHandlerBase
	{
		RequestHandlerBase() {}

		RequestHandlerBase(std::shared_ptr<TQueue> cq
			, RpcCallbackFunction callback_)
			: completion_queue(cq), callback(callback_)
		{	}
		std::shared_ptr<TQueue> completion_queue;
		RpcCallbackFunction callback;
	};


	struct ServerRequestHandler : RequestHandlerBase<grpc::ServerCompletionQueue>
	{
		ServerRequestHandler( std::shared_ptr<grpc::ServerCompletionQueue> cq
			                  , RpcCallbackFunction callback_) 
			                  : RequestHandlerBase(cq, callback_)
		{	}

	};

	struct ClientRequestHandler : RequestHandlerBase<grpc::CompletionQueue>
	{
		ClientRequestHandler( std::shared_ptr<grpc::CompletionQueue> cq
			                  , RpcCallbackFunction callback_)
			                  : RequestHandlerBase(cq, callback_)
		{	}

	};

	typedef std::map<std::string, ClientRequestHandler> ClientRequestHandlers;
	typedef std::list<ServerRequestHandler> ServerRequestHandlers;


	namespace utils
	{
		inline void set_deadline(grpc::ClientContext& context, uint32_t seconds)
		{
			auto deadline =
				std::chrono::system_clock::now() + std::chrono::seconds(seconds);
			context.set_deadline(deadline);
		}

		inline
		void set_metadata(grpc::ClientContext& context
			, const std::vector<std::pair<std::string, std::string>>& metadata) 
		{
			for (auto item : metadata)
				context.AddMetadata(item.first, item.second);
		}

		inline
		void set_metadata(grpc::ClientContext& context
			, const std::pair<std::string, std::string>& metadata) 	
		{
			context.AddMetadata(metadata.first, metadata.second);
		}


		inline void to_bytes( const DataTypes::GetRequest& request
			                  , DataTypes::MessageBytes& message)
		{
			std::string bytes;
			request.SerializeToString(&bytes);
			message.set_data(bytes);
			message.set_type(DataTypes::DataType::GetRequestType);
		}

		inline void to_bytes(const DataTypes::CommitRequest& request
			, DataTypes::MessageBytes& message)
		{
			std::string bytes;
			request.SerializeToString(&bytes);
			message.set_data(bytes);
			message.set_type(DataTypes::DataType::CommitRequestType);
		}

		template <typename T>
		grpc::CompletionQueue* get_completion_queue(const ClientRequestHandlers& handlers)
		{
			auto it = handlers.find(typeid(T).name());
			if (it == handlers.end())
				return nullptr;
			return it->second.completion_queue.get();
		}
		
		template <typename T>
		T get_result(std::promise<T>& promise
			 , std::chrono::milliseconds time_duration = std::chrono::milliseconds(50))
		{
			auto future = promise.get_future();

			auto try_count = 0;
			while (future.wait_for(time_duration) == std::future_status::timeout
				&& try_count < 10)			
				try_count++;
			auto failed = try_count >= 10;
			if (failed)
				throw std::exception("timeout exception");
			
			return future.get();
		}
	}

}

#endif