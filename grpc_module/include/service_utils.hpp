#ifndef ServiceUtils_INCLUDED
#define ServiceUtils_INCLUDED

#include <datatypes/queries.pb.h>
#include <datatypes/commands.pb.h>
#include <include/grpc++/impl/codegen/completion_queue.h>
#include <future>

namespace grpc_services
{
	typedef Services::UnitService::AsyncService AsyncService;
	
	typedef std::function<void()> RpcCallbackFunction;

	typedef std::pair<std::shared_ptr<grpc::CompletionQueue>
		, RpcCallbackFunction> CallHandler;

	typedef std::map<std::string, CallHandler> CallHandlers;

	namespace utils
	{
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
		grpc::CompletionQueue* get_completion_queue(const CallHandlers& handlers)
		{
			auto it = handlers.find(typeid(T).name());
			if (it == handlers.end())
				return nullptr;
			return it->second.first.get();
		}
		
		template <typename T>
		T get_result(std::promise<T>& promise
			 , std::chrono::milliseconds time_duration = std::chrono::milliseconds(200))
		{
			auto future = promise.get_future();
			future.wait_for(time_duration);
			return future.get();
		}
	}

}

#endif