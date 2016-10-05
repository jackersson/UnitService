#ifndef IServerManager_Included
#define IServerManager_Included

#include <contracts/common/ilifecycle.hpp>

namespace contracts
{
	namespace services
	{
		class IServiceManager : public common::ILifecycle
		{
		public:
			virtual ~IServiceManager() {}
		};
	}
}

#endif
