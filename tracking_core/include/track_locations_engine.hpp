#ifndef TrackLocationsEngine_Included
#define TrackLocationsEngine_Included

#include "track_location.hpp"
#include <contracts/locations/itrack_location_coordinator.hpp>
#include "track_locations_container.hpp"
#include <network_utils.hpp>
#include <contracts/devices/idevices_container.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace tracking
{
	namespace locations
	{
		class TrackLocationsEngine 
			: public contracts::locations::ITrackLocationsEngine
			, public contracts::devices::IDevicesSet
		{
		public:
			explicit TrackLocationsEngine(  contracts::IServiceContext* context)
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
			
			void update(const data_model::Location& location) override {
				if ( mac_address_valid(location) )
					upsert(location);
			}

			void update_with(const std::vector<data_model::Location>& locations) override
			{
				std::set<data_model::Key> target_locations_set;
				for (auto location : locations)
				{
					if ( !location.id().is_empty() 
						|| !mac_address_valid(location))
						continue;

					target_locations_set.insert(location.id());
					upsert(location);
				}

				std::vector<data_model::Key> to_remove;
				if (container_.get_difference(target_locations_set, to_remove))
					remove_range(to_remove);
			}


			bool contains( const std::string& device_name
				           , data_model::DeviceType dev_type) const override
			{
				return container_.contains(device_name, dev_type);
			}
			
			bool contains(const data_model::Location& location) const override
			{
				if (location.id().is_empty())
					return false;	

				return container_.contains(location.id());
			}

			bool contains(const data_model::Key& uuid) const 	{
				return container_.contains(uuid);
			}

			bool grant_access(const data_model::Location& location) override
			{
				if (location.id().is_empty())
				{
					logger_.error("Grant access failed. Location not found");
					return false;
				}
				try
				{
					auto it = container_.find(location.id());
					it->access_coordinator().set_state(data_model::AccessState::Granted);
				}
				catch (std::exception& exception) {
					logger_.error("Grant access failed. {0}", exception.what());
					return false;
				}
				return true;
			}
			
			void upsert( const data_model::Location& location  )
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

			void remove(const data_model::Location& location)
			{
				if (location.id().is_empty())
				{
					logger_.error("Error while remove track location. Uid not valid");
					return;
				}

				remove(location.id());
			}

			void remove(const data_model::Key& uuid)
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

			void remove_range(std::vector<data_model::Key>& uuids)
			{
				for (auto item : uuids)
					remove(item);
			}

			size_t size() const override
			{
				return container_.size();
			}

		private:
			bool mac_address_valid(const data_model::Location& location) const
			{
				if (location.unit_mac_address() != local_macaddress_)
				{
					logger_.error( "Location (id {0}) macaddress is not valid for unit service"
					            	, to_string(location.id().guid()));
					return false;
				}
				return true;
			}
			
			std::string                 local_macaddress_;
			contracts::IServiceContext* context_         ;
			TrackLocationsContainer     container_       ;

			mutable contracts::logging::Logger logger_;
		};
	}
}

#endif
