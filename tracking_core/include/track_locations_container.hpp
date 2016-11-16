#ifndef TrackLocationsContainer_Included
#define TrackLocationsContainer_Included

#include <concurrent_containers.hpp>
#include "track_location.hpp"
#include <set>

namespace data_model
{
	class Key;
}

namespace tracking
{
	namespace locations
	{
		class TrackLocationsContainer 
		{
		public:		 
			void add   (contracts::locations::ILocationPtr location);
			void remove(contracts::locations::ILocationPtr location);

			void remove(const data_model::Key& uuid);

			contracts::locations::ILocationPtr find(const data_model::Key& key);

			bool contains( contracts::locations::ILocationPtr location) const;
			bool contains( const data_model::Key& uuid) const;
			bool contains( const data_model::DeviceId& device_name
				           , data_model::DeviceType dev_type) const;

			void clear();

			//returns true if there is any difference
			bool get_difference( const std::set<data_model::Key>& items
			                 	 , std::vector<data_model::Key>& difference);

			size_t size()	const;

		private:		
			std::set<data_model::Key> keys_;

			//TODO not mutable but const contains, find
			mutable concurrent::containers::ConcurrentMap<data_model::Key
				, contracts::locations::ILocationPtr> track_locations_;
		};
	}
}

#endif
