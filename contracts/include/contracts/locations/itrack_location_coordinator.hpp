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

			virtual const std::vector<ILocation> Locations() = 0;
		};
	}
}

#endif
