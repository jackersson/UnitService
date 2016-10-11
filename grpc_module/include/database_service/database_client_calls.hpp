#ifndef DatabaseClientCall_INCLUDED
#define DatabaseClientCall_INCLUDED

#include <datatypes/data.pb.h>
#include <include/grpc++/impl/codegen/client_context.h>
#include <future>

namespace grpc_services
{

	struct AsyncGetRequestCall {
		DataTypes::MessageBytes response;

		void parsed_response() 
		{
			if (response.type() != DataTypes::DataType::GetResponseType)
			{
				promise_.set_exception(std::make_exception_ptr("Invalid data type"));
				return;
			}
			auto gr = std::make_shared<Services::GetResponse>();
			gr->ParseFromString(response.data());
			promise_.set_value(gr);
		}

		std::promise<std::shared_ptr<Services::GetResponse>> promise_;
	
		grpc::ClientContext context;

		grpc::Status status;

		std::unique_ptr<grpc::ClientAsyncResponseReader<DataTypes::MessageBytes>> reader;
	};


	typedef std::function<void(std::shared_ptr<Services::CommitResponse>)> CommitCallbackFunction;

	struct AsyncCommitRequestCall {
		DataTypes::MessageBytes response;

		std::shared_ptr<Services::CommitResponse> parsed_response() const
		{
			if (response.type() != DataTypes::DataType::CommitResponseType)
				return nullptr;

			auto gr = std::make_shared<Services::CommitResponse>();
			gr->ParseFromString(response.data());
			return gr;
		}

		grpc::ClientContext context;

		CommitCallbackFunction callback;

		grpc::Status status;

		std::unique_ptr<grpc::ClientAsyncResponseReader<DataTypes::MessageBytes>> reader;
	};
}

#endif