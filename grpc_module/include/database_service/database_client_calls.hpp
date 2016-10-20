#ifndef DatabaseClientCall_INCLUDED
#define DatabaseClientCall_INCLUDED

#include <datatypes/queries.pb.h>
#include <datatypes/commands.pb.h>
#include <include/grpc++/impl/codegen/client_context.h>
#include <future>

namespace grpc_services
{
	template <typename TResponse>
	struct AsyncCallBase
	{
		TResponse           response;
		grpc::ClientContext context ;
		grpc::Status        status  ;
	};

	struct AsyncGetRequestCall : AsyncCallBase<DataTypes::MessageBytes> {
		void process()
		{
			if (response.type() != DataTypes::DataType::GetResponseType)
			{
				promise.set_exception(std::make_exception_ptr("Invalid data type"));
				return;
			}
			auto gr = std::make_shared<DataTypes::GetResponse>();
			gr->ParseFromString(response.data());
			promise.set_value(gr);
		}

		std::promise<std::shared_ptr<DataTypes::GetResponse>> promise;
	
		std::unique_ptr<grpc::ClientAsyncResponseReader<DataTypes::MessageBytes>> reader;
	};



	struct AsyncCommitRequestCall : AsyncCallBase<DataTypes::MessageBytes> {

		void process()
		{
			if (response.type() != DataTypes::DataType::CommitResponseType)
			{
				promise.set_exception(std::make_exception_ptr("Invalid data type"));
				return;
			}

			auto gr = std::make_shared<DataTypes::CommitResponse>();
			gr->ParseFromString(response.data());
			promise.set_value(gr);
		}

		std::promise<std::shared_ptr<DataTypes::CommitResponse>> promise;
		std::unique_ptr<grpc::ClientAsyncResponseReader<DataTypes::MessageBytes>> reader;
	};
}

#endif