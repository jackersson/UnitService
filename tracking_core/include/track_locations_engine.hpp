#ifndef TrackLocationsEngine_Included
#define TrackLocationsEngine_Included

#include <datatypes/location.pb.h>
#include <contracts/iunit_context.hpp>

#include <boost/uuid/uuid.hpp>

#include "track_location.hpp"
#include <contracts/locations/itrack_location_coordinator.hpp>
#include <data/data_utils.hpp>
#include "track_locations_container.hpp"
#include <network_utils.hpp>
#include <contracts/devices/idevices_container.hpp>

namespace tracking
{
	namespace locations
	{
		class TrackLocationsEngine 
			: public contracts::locations::ITrackLocationsEngine
			, public contracts::devices::IDevicesSet
		{
		public:
			explicit TrackLocationsEngine(  contracts::IUnitContext* context)
				                            : context_(context)
			{				
				local_macaddress_ = utils::network::get_mac_address();
			}

			virtual ~TrackLocationsEngine()	{
				TrackLocationsEngine::de_init();
			}

			void init() override
			{
				
			}

			void de_init() override
			{
				
			}
			
			void update(const DataTypes::Location& location) override {
				if ( mac_address_valid(location) )
					upsert(location);
			}

			void update_with(const std::vector<DataTypes::Location>& locations) override
			{
				std::set<boost::uuids::uuid> target_locations_set;
				for (auto location : locations)
				{
					boost::uuids::uuid uid;
					if ( !contracts::data::get_guid(location.id(), uid)
						 || !mac_address_valid(location))
						continue;

					target_locations_set.insert(uid);				
					upsert(location, uid);
				}

				std::vector<boost::uuids::uuid> to_remove;
				if (container_.get_difference(target_locations_set, to_remove))
					remove_range(to_remove);
			}


			bool contains( const std::string& device_name
				           , DataTypes::DeviceType dev_type) const override
			{
				return container_.contains(device_name, dev_type);
			}
			
			bool contains(const DataTypes::Location& location) const override
			{
				boost::uuids::uuid uuid;
				if (!contracts::data::get_guid(location.id(), uuid))
					return false;

				return container_.contains(uuid);
			}

			bool contains(const boost::uuids::uuid& uuid) const 	{
				return container_.contains(uuid);
			}

			bool grant_access(const DataTypes::Location& location) override
			{
				//TODO refactor
				boost::uuids::uuid uuid;
				if (!contracts::data::get_guid(location.id(), uuid))
				{
					logger_.error("Grant access failed. Location not found");
					return false;
				}
				try
				{
					auto it = container_.find(uuid);
					it->access_coordinator().set_state(DataTypes::AccessState::Granted);
				}
				catch (std::exception& exception) {
					logger_.error("Grant access failed. {0}", exception.what());
					return false;
				}
				return true;
			}


			void upsert(const DataTypes::Location& location)
			{
				boost::uuids::uuid uid;
				if (!contracts::data::get_guid(location.id(), uid))
				{
					logger_.error("Error while add location. Uid not valid");
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
					logger_.error("Error while remove track location. Uid not valid");
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
					logger_.error("{0}", exception.what());
				}
			}

			void remove_range(std::vector<boost::uuids::uuid>& uuids)
			{
				for (auto item : uuids)
					remove(item);
			}

			size_t size() const override
			{
				return container_.size();
			}

		private:
			bool mac_address_valid(const DataTypes::Location& location) const
			{
				if (location.unit_mac_address() != local_macaddress_)
				{
					logger_.error("Location (id {0}) macaddress is not valid for unit service"
						, contracts::data::to_string(location.id()));
					return false;
				}
				return true;
			}


			std::string local_macaddress_;
			contracts::IUnitContext* context_;
			TrackLocationsContainer container_;

			mutable contracts::logging::Logger logger_;
		};
	}
}

#endif
