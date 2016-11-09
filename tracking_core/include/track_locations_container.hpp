#ifndef TrackLocationsContainer_Included
#define TrackLocationsContainer_Included

#include <concurrent_containers.hpp>
#include "track_location.hpp"
#include <set>

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

			void add(contracts::locations::ILocationPtr location)	
			{
				auto& uuid = location->location().id();
				if (!track_locations_.contains(uuid))
				{
					track_locations_.insert(uuid, location);
					keys_.insert(uuid);
				}
			}

			void remove(contracts::locations::ILocationPtr location)
			{
				auto& id = location->location().id();
				if (!track_locations_.contains(id))
					remove(id);
			}

			void remove(const data_model::Key& uuid)
			{			
					track_locations_.remove(uuid);
					keys_.erase(uuid);				
			}

			contracts::locations::ILocationPtr find(const data_model::Key& key) 
			{
				return	track_locations_.find(key);
			}

			bool contains(contracts::locations::ILocationPtr location) const {
				return track_locations_.contains(location->location().id());
			}

			bool contains(const data_model::Key& uuid) const	{
				return track_locations_.contains(uuid);
			}

			bool contains( const std::string& device_name
				           , data_model::DeviceType dev_type) const
			{
				switch(dev_type)
				{
				case data_model::None_Type: break;
				case data_model::CardReader:
					return (access_devices_.find(device_name) != access_devices_.end());
				case data_model::Capture:
					return (video_devices_.find(device_name) != video_devices_.end());
				default: 
					break;
				}
				return false;
			}

			void clear()
			{
				track_locations_.clear();
				keys_.clear();
			}

			//returns true if there is any difference
			bool get_difference( const std::set<data_model::Key>& items
			                   , std::vector<data_model::Key>& difference)
			{
				std::set_difference(keys_.begin(), keys_.end()
					                 , items.begin(), items.end()
					                 , std::inserter(difference, difference.begin()));
				return difference.size() > 0;
			}			

			size_t size()	const {
				return track_locations_.size();
			}

		private:		
			std::set<data_model::Key> keys_;

			std::set<std::string> access_devices_;
			std::set<std::string> video_devices_ ;

			//TODO not mutable but const contains, find
			mutable concurrent::containers::ConcurrentMap<data_model::Key
				, contracts::locations::ILocationPtr> track_locations_;
		};
	}
}

#endif
