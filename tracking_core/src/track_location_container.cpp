#include "track_locations_container.hpp"

#include <data/models/location.hpp>
#include <iterator>

using namespace locations;

namespace tracking
{	
	typedef std::pair<data_model::Key, ILocationPtr> LocationPair;
	 
	void TrackLocationsContainer::add(ILocationPtr location)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		auto& uuid = location->location().id();
		if (!contains(uuid))
		{
			map_.insert(LocationPair(uuid, location));
			keys_.insert(uuid);
		}
	}

	void TrackLocationsContainer::remove(ILocationPtr location)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		auto& id = location->location().id();
		if (contains(id))
			remove(id);
	}

	void TrackLocationsContainer::remove(const data_model::Key& uuid)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		map_.erase(uuid);
		keys_.erase(uuid);
	}

	ILocationPtr TrackLocationsContainer::find(const data_model::Key& key)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		auto ptr = map_.find(key);
		return ptr == map_.end() ? nullptr : ptr->second;
	}

	bool TrackLocationsContainer::contains(ILocationPtr location) const {
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		return map_.find(location->location().id()) != map_.end();
	}

	bool TrackLocationsContainer::contains(const data_model::Key& uuid) const {
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		return map_.find(uuid) != map_.end();
	}
			
	bool 
		TrackLocationsContainer::contains( const data_model::DeviceId& device_name
		                                 , data_model::DeviceType dev_type) const
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		if (device_name.is_empty())
			return false;
		switch (dev_type)
		{
		case data_model::None_Type: break;
		case data_model::CardReader:
			for (auto& item : map_)
			{
				auto& dev = item.second->location().access_device();
				if (!dev.is_empty() && dev == device_name)
					return true;
			}			
		case data_model::Capture:
			for (auto item : map_)
			{
				auto& dev = item.second->location().capture_device();
				if (!dev.is_empty() && dev == device_name)
					return true;
			}
		default:
			break;
		}
		return false;
	}
	
	void TrackLocationsContainer::clear()
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);

		for (auto& it : keys_)
		{
			auto loc = find(it);
			if (loc != nullptr)
			  loc->stop();
			map_.erase(it);
		}
		keys_.clear();
	}

	//returns true if there is any difference
	bool TrackLocationsContainer::get_difference
	  ( const std::set<data_model::Key>& items
		, std::vector<data_model::Key>& difference)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		std::set_difference(keys_.begin(), keys_.end()
			, items.begin(), items.end()
			, std::inserter(difference, difference.begin()));
		return difference.size() > 0;
	}

	size_t TrackLocationsContainer::size()	const {
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		return map_.size();
	}	
	
}

