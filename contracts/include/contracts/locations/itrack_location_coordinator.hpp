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

			virtual bool grant_access(const DataTypes::Location& location) = 0;

			//TODO maybe not need
			virtual const std::vector<ILocationPtr>& locations() const = 0;
		};

		typedef std::shared_ptr<ITrackLocationsCoordinator> ITrackLocationsCoordinatorPtr;
	}
}

#endif
