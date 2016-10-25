#ifndef TrackLocationsContainer_Included
#define TrackLocationsContainer_Included

#include <datatypes/location.pb.h>
#include <concurrent_containers.hpp>
#include <boost/uuid/uuid.hpp>

#include "track_location.hpp"

namespace tracking
{
	namespace locations
	{
		class TrackLocationsContainer 
		{
		public:
			explicit TrackLocationsContainer()				
			{}	

			virtual ~TrackLocationsContainer() {}

			void add(contracts::locations::ILocationPtr location)	{
				auto uuid = location->id();
				if (!track_locations_.contains(uuid))
				{
					track_locations_.insert(uuid, location);
					uuids_.insert(uuid);
				}
			}

			void remove(contracts::locations::ILocationPtr location) {
				auto uuid = location->id();
				if (!track_locations_.contains(uuid))
					remove(uuid);
			}

			void remove(const boost::uuids::uuid& uuid) {			
					track_locations_.remove(uuid);
					uuids_.erase(uuid);				
			}

			contracts::locations::ILocationPtr find(const boost::uuids::uuid& uuid) {				
				return	track_locations_.find(uuid);
			}

			bool contains(contracts::locations::ILocationPtr location) const {
				auto uuid = location->id();
				return track_locations_.contains(uuid);
			}

			bool contains(const boost::uuids::uuid& uuid) const	{
				return track_locations_.contains(uuid);
			}

			void clear()
			{
				track_locations_.clear();
				uuids_.clear();
			}

			//returns true if there is any difference
			bool get_difference( const std::set<boost::uuids::uuid>& items
			                   , std::vector<boost::uuids::uuid>& difference)
			{
				std::set_difference( uuids_.begin(), uuids_.end()
					                 , items.begin(), items.end()
					                 , std::inserter(difference, difference.begin()));
				return difference.size() > 0;
			}			

			size_t size()	const {
				return track_locations_.size();
			}

		private:		
			std::set<boost::uuids::uuid> uuids_;
			//TODO not mutable but const contains, find
			mutable concurrent::containers::ConcurrentMap<boost::uuids::uuid
				, contracts::locations::ILocationPtr> track_locations_;
		};

	}
}

#endif
