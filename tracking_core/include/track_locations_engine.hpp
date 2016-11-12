#ifndef TrackLocationsEngine_Included
#define TrackLocationsEngine_Included

#include <contracts/locations/itrack_location_coordinator.hpp>
#include "track_locations_container.hpp"
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
			explicit TrackLocationsEngine(contracts::IServiceContext* context);               

			virtual ~TrackLocationsEngine();

			void init   () override;
			void de_init() override;
			
			void update(const data_model::Location& location) override;
			void add   (const data_model::Location& location) override;

			void update_with(const std::vector<data_model::Location>& locations) override;
			
			bool contains( const std::string& device_name
				           , data_model::DeviceType dev_type) const override;
			
			bool contains(const data_model::Location& location) const override;
			bool contains(const data_model::Key& uuid         ) const;

			bool grant_access(const data_model::Location& location) override;
			
			void upsert(const data_model::Location& location);

			void remove(const data_model::Location& location) override;
			void remove(const data_model::Key& uuid);

			void remove_range(std::vector<data_model::Key>& uuids);

			size_t size() const override;

		private:
			bool mac_address_valid(const data_model::Location& location) const;
			
			std::string                 local_macaddress_;
			contracts::IServiceContext* context_         ;
			TrackLocationsContainer     container_       ;

			mutable contracts::logging::Logger logger_;
		};
	}
}

#endif
