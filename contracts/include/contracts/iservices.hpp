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

			virtual IFileStorageApiPtr   file_storage()   = 0;
			virtual IDatabaseApiPtr      database()       = 0;
			virtual ICoordinatorApiPtr   coordinator()    = 0;
			virtual IFacialServiceApiPtr facial_service() = 0;
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
