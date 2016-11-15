#include "track_locations_engine.hpp"

#include <network_utils.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <data/models/location.hpp>
#include <data/models/visit_record.hpp>

using namespace data_model;

namespace tracking
{
	namespace locations
	{		
		TrackLocationsEngine::TrackLocationsEngine(contracts::IServiceContext* context)
			: context_(context)
		{
			local_macaddress_ = utils::network::get_mac_address();
		}

		TrackLocationsEngine::~TrackLocationsEngine() {
			TrackLocationsEngine::de_init();
		}

		void TrackLocationsEngine::init()  {}

		void TrackLocationsEngine::de_init()  {
			container_.clear();
		}

		void TrackLocationsEngine::update(const Location& location)  {
			if (mac_address_valid(location))
				upsert(location);
		}

		void TrackLocationsEngine::add(const Location& location)  {
			if (mac_address_valid(location))
				upsert(location);
		}

		void TrackLocationsEngine::update_with(const std::vector<Location>& locations)
		{
			std::set<Key> target_locations_set;

			for (auto location : locations)
			{
				if (location.id().is_empty()
					|| !mac_address_valid(location))
					continue;

				target_locations_set.insert(location.id());
				upsert(location);
			}

			std::vector<Key> to_remove;
			if (container_.get_difference(target_locations_set, to_remove))
				remove_range(to_remove);
		}


		bool TrackLocationsEngine::contains(const DeviceId& device_name
			, DeviceType dev_type) const 
		{
			return container_.contains(device_name, dev_type);
		}

		bool TrackLocationsEngine::contains(const Location& location) const
		{
			if (location.id().is_empty())
				return false;

			return container_.contains(location.id());
		}

		bool TrackLocationsEngine::contains(const Key& uuid) const {
			return container_.contains(uuid);
		}

		bool TrackLocationsEngine::grant_access(const Location& location)
		{
			if (location.id().is_empty())
			{
				logger_.error("Grant access failed. Location not found");
				return false;
			}
			try
			{
				auto it = container_.find(location.id());
				it->access_coordinator().set_state(AccessState::Granted);
			}
			catch (std::exception& exception) {
				logger_.error("Grant access failed. {0}", exception.what());
				return false;
			}
			return true;
		}

		void TrackLocationsEngine::upsert(const Location& location)
		{
			if (location.id().is_empty())
			{
				logger_.error("Error while add location. Uid not valid");
				return;
			}

			try {
				auto existing = container_.find(location.id());
				existing->update(location);
			}
			catch (std::exception&)
			{
				auto track_location = std::make_shared<TrackLocation>(location, context_);
				container_.add(track_location);
			}
		}

		void TrackLocationsEngine::remove(const Location& location)
		{
			if (location.id().is_empty())
			{
				logger_.error("Error while remove track location. Uid not valid");
				return;
			}

			remove(location.id());
		}

		void TrackLocationsEngine::remove(const Key& uuid)
		{
			try {
				auto existing = container_.find(uuid);
				existing->stop();
				container_.remove(uuid);
			}
			catch (std::exception& exception) {
				logger_.error("{0}", exception.what());
			}
		}

		void TrackLocationsEngine::remove_range(std::vector<Key>& uuids)
		{
			for (auto item : uuids)
				remove(item);
		}

		size_t TrackLocationsEngine::size() const
		{
			return container_.size();
		}

		bool TrackLocationsEngine::mac_address_valid(const Location& location) const
		{
			if (location.unit_mac_address() != local_macaddress_)
			{
				logger_.error("Location (id {0}) macaddress is not valid for unit service"
					, to_string(location.id().guid()));
				return false;
			}
			return true;
		}
	}
}

