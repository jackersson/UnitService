#include "track_locations_container.hpp"

#include <data/models/location.hpp>

using namespace contracts::locations;

namespace tracking
{
	namespace locations
	{	
		void TrackLocationsContainer::add(ILocationPtr location)
		{
			auto& uuid = location->location().id();
			if (!track_locations_.contains(uuid))
			{
				track_locations_.insert(uuid, location);
				keys_.insert(uuid);
			}
		}

		void TrackLocationsContainer::remove(ILocationPtr location)
		{
			auto& id = location->location().id();
			if (!track_locations_.contains(id))
				remove(id);
		}

		void TrackLocationsContainer::remove(const data_model::Key& uuid)
		{
			track_locations_.remove(uuid);
			keys_.erase(uuid);
		}

		ILocationPtr TrackLocationsContainer::find(const data_model::Key& key)
		{
			return	track_locations_.find(key);
		}

		bool TrackLocationsContainer::contains(ILocationPtr location) const {
			return track_locations_.contains(location->location().id());
		}

		bool TrackLocationsContainer::contains(const data_model::Key& uuid) const {
			return track_locations_.contains(uuid);
		}

		bool TrackLocationsContainer::contains(const std::string& device_name
			, data_model::DeviceType dev_type) const
		{
			switch (dev_type)
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

		void TrackLocationsContainer::clear()
		{
			track_locations_.clear();
			keys_.clear();
		}

		//returns true if there is any difference
		bool TrackLocationsContainer::get_difference(const std::set<data_model::Key>& items
			, std::vector<data_model::Key>& difference)
		{
			std::set_difference(keys_.begin(), keys_.end()
				, items.begin(), items.end()
				, std::inserter(difference, difference.begin()));
			return difference.size() > 0;
		}

		size_t TrackLocationsContainer::size()	const {
			return track_locations_.size();
		}	
	}
}

