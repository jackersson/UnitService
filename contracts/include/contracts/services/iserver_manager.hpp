#ifndef IServerManager_Included
#define IServerManager_Included

#include <contracts/common/ilifecycle.hpp>
#include <contracts/services/idatabase_api.hpp>
#include <contracts/services/icoordinator_api.hpp>

namespace contracts
{
	namespace services
	{
		class IServiceManager : public common::ILifecycle
		{
		public:
			virtual ~IServiceManager() {}
		};

		class IClients
		{
		public:
			virtual ~IClients() {}

			virtual IDatabaseApiPtr    database   () = 0;
			virtual ICoordinatorApiPtr coordinator() = 0;
		};

		typedef std::shared_ptr<IClients> IClientsPtr;

		class IServices : public common::IModule
		{
		public:

			virtual IClientsPtr clients() = 0;
		};
	}
}

#endif
