#ifndef TrackLocationsEngine_Included
#define TrackLocationsEngine_Included

#include <datatypes/location.pb.h>
#include <contracts/iunit_context.hpp>

#include <boost/uuid/uuid.hpp>

#include "track_location.hpp"
#include <contracts/locations/itrack_location_coordinator.hpp>
#include <contracts/data/data_utils.hpp>
#include "track_locations_container.hpp"

namespace tracking
{
	namespace locations
	{
		class TrackLocationsEngine : public contracts::locations::ITrackLocationsEngine
		{
		public:
			explicit TrackLocationsEngine(  contracts::IUnitContextPtr context)
				                            : context_(context)
			{}

			virtual ~TrackLocationsEngine() {}

			void update(const DataTypes::Location& location) override {
				//TODO check if macaddress valid for this locations
				upsert(location);
			}

			void update_with(const std::vector<DataTypes::Location>& locations) override
			{
				//TODO check if macaddress valid for this locations
				std::set<boost::uuids::uuid> target_locations_set;
				for (auto location : locations)
				{
					boost::uuids::uuid uid;
					if (!contracts::data::get_guid(location.id(), uid))
						continue;

					target_locations_set.insert(uid);
					if (contains(uid))
						upsert(location, uid);
				}

				std::vector<boost::uuids::uuid> to_remove;
				if (container_.get_difference(target_locations_set, to_remove))
					remove_range(to_remove);
			}

			
			bool contains(const DataTypes::Location& location)
			{
				boost::uuids::uuid uuid;
				if (!contracts::data::get_guid(location.id(), uuid))
					return false;

				return container_.contains(uuid);
			}

			bool contains(const boost::uuids::uuid& uuid)	{
				return container_.contains(uuid);
			}

			bool grant_access(const DataTypes::Location& location) override
			{
				//TODO refactor
				boost::uuids::uuid uuid;
				if (!contracts::data::get_guid(location.id(), uuid))
				{
					context_->logger()->error("Grant access failed. Location not found");
					return false;
				}
				try
				{
					auto it = container_.find(uuid);
					it->access_coordinator().set_state(DataTypes::AccessState::Granted);
				}
				catch (std::exception& exception) {
					context_->logger()->error("Grant access failed. {0}", exception.what());
					return false;
				}
				return true;
			}


			void upsert(const DataTypes::Location& location)
			{
				boost::uuids::uuid uid;
				if (!contracts::data::get_guid(location.id(), uid))
				{
					context_->logger()->error("Error while add location. Uid not valid");
					return;
				}

				upsert(location, uid);
			}

			void upsert( const DataTypes::Location& location
				         , const boost::uuids::uuid& uid)
			{

				try {
					auto existing = container_.find(uid);
					existing->update(location);
				}
				catch (std::exception&)
				{
					auto track_location = std::make_shared<TrackLocation>(location, context_);
					container_.add(track_location);				
				}
			}

			void remove(const DataTypes::Location& location)
			{
				boost::uuids::uuid uid;
				if (!contracts::data::get_guid(location.id(), uid))
				{
					context_->logger()->error("Error while remove track location. Uid not valid");
					return;
				}

				remove(uid);
			}

			void remove(boost::uuids::uuid& uuid)
			{
				try {
					auto existing = container_.find(uuid);
					existing->stop();
					container_.remove(uuid);
				}
				catch (std::exception& exception) {
					context_->logger()->error("{0}", exception.what());
				}
			}

			void remove_range(std::vector<boost::uuids::uuid>& uuids)
			{
				for (auto item : uuids)
					remove(item);
			}

		private:
			contracts::IUnitContextPtr context_;
			TrackLocationsContainer container_;	
		};
	}
}

#endif
