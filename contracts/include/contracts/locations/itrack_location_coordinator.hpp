#ifndef ITrackLocationsCoordinator_Included
#define ITrackLocationsCoordinator_Included

#include <vector>
#include "ilocation.hpp"

namespace contracts
{
	namespace locations
	{
		class ITrackLocationsCoordinator
		{
		public:
			virtual ~ITrackLocationsCoordinator() {}

			virtual const std::vector<ILocationPtr>& locations() const = 0;
		};

		typedef std::shared_ptr<ITrackLocationsCoordinator> ITrackLocationsCoordinatorPtr;
	}
}

#endif
