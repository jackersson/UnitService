#ifndef CoordinatorClientCall_INCLUDED
#define CoordinatorClientCall_INCLUDED

#include <google/protobuf/empty.pb.h>
#include <future>
#include <database_service/database_client_calls.hpp>

namespace grpc_services
{
	struct AsyncEmptyCall : AsyncCallBase<google::protobuf::Empty>
	{
		void process_response() {
			promise.set_value(true);
		}

		std::promise<bool>  promise;
		std::unique_ptr<grpc::ClientAsyncResponseReader<google::protobuf::Empty>> reader;
	};

	struct AsyncConnectCall   : AsyncEmptyCall {};

	struct AsyncHeartbeatCall : AsyncEmptyCall {};

	struct AsyncUpdateDevicesCall : AsyncEmptyCall{};
	


	
}

#endif