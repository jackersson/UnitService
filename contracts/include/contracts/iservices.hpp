#ifndef IServerManager_Included
#define IServerManager_Included

#include <common/ilifecycle.hpp>
#include <services/idatabase_api.hpp>
#include <services/icoordinator_api.hpp>
#include <services/ifile_storage_api.hpp>
#include <services/ifacial_service_api.hpp>

namespace contracts
{
	namespace services
	{
		class IClients
		{
		public:
			virtual ~IClients() {}

			//virtual IFileStorageApiPtr   file_storage()   = 0;
			virtual IDatabaseApi*      database()       = 0;
			virtual ICoordinatorApi*   coordinator()    = 0;
			virtual IFacialServiceApi* facial_service() = 0;
		};

		typedef std::shared_ptr<IClients> IClientsPtr;

		class IServices : public common::IModule
			              //, public common::ILifecycle
		{
		public:

			virtual IClientsPtr clients() = 0;
		};
	}
}

#endif
