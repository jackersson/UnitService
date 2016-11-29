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
	class TrackLocationsContainer 
	{
	public:		 
		void add   (::locations::ILocationPtr location);
		void remove(::locations::ILocationPtr location);

		void remove(const data_model::Key& uuid);

		::locations::ILocationPtr find(const data_model::Key& key);

		bool contains( ::locations::ILocationPtr location) const;
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

		mutable std::recursive_mutex mutex_;

		std::map<data_model::Key, ::locations::ILocationPtr>
			map_;
	};	
}

#endif
