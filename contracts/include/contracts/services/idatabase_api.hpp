#ifndef IDatabaseApi_INCLUDED
#define IDatabaseApi_INCLUDED

#include <datatypes/queries.pb.h>
#include <datatypes/commands.pb.h>

namespace contracts
{
	namespace services
	{
		class IDatabaseApi
		{
		public:
			virtual ~IDatabaseApi() {}

			virtual std::shared_ptr<DataTypes::GetResponse>
				get(const DataTypes::GetRequest& request) = 0;

			virtual std::shared_ptr<DataTypes::CommitResponse>
				commit(const DataTypes::CommitRequest& request) = 0;
		};

		typedef std::shared_ptr<IDatabaseApi> IDatabaseApiPtr;
	}
}

#endif
