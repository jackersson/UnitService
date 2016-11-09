#ifndef IServerManager_Included
#define IServerManager_Included

#include <services/icoordinator_api.hpp>
#include <services/ifacial_service_api.hpp>
#include <services/iservice.hpp>
#include <data/irepository.hpp>

namespace contracts
{
	namespace services
	{		
		class IServices : public IService
		{
		public:
			virtual data::AbstractDataContextContainer* database      () = 0;
			virtual ICoordinatorApi*                    coordinator   () = 0;
			virtual IFacialServiceApi*                  facial_service() = 0;
		};
	}
}

#endif
