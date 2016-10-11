#ifndef IDatabaseApi_INCLUDED
#define IDatabaseApi_INCLUDED

#include <services/database_service.grpc.pb.h>

namespace contracts
{
	namespace services
	{
		class IDatabaseApi
		{
		public:
			virtual ~IDatabaseApi() {}

			virtual std::shared_ptr<Services::GetResponse>
				get(const Services::GetRequest& request) = 0;

			virtual std::shared_ptr<Services::CommitResponse>
				commit(const Services::CommitRequest& request) = 0;
		};

		typedef std::shared_ptr<IDatabaseApi> IDatabaseApiPtr;
	}
}

#endif
