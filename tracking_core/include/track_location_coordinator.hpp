#ifndef TrackLocationCoordinator_Included
#define TrackLocationCoordinator_Included
#include <vector>
#include <datatypes/location.pb.h>
#include <concurrent_containers.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>

#include "track_location.hpp"
#include <containers_utils.hpp>

namespace tracking
{
	namespace locations
	{
		class TrackLocationCoordinator
		{
		public:
			virtual ~TrackLocationCoordinator() {}

		private:
			void on_data_changed()
			{
				std::vector<DataTypes::Location> data; //TODO implement
				//TODO check with current mac address

				for (auto location : data)
				{
					auto guid = to_uuid(location.id());
					
					try {
						auto existing = track_locations_.find(guid);
						existing.update(location);
					}
					catch (std::exception& exception)
					{
						TrackLocation track_location(location);
						track_locations_.insert(guid, track_location);
					}					
				}

				locations_.clear();
				
				for (auto item : track_locations_)
				{
					auto track_location = item.second;
					auto location = track_location.location();
					if (!utils::containers::contains(data, location))
					{
						track_location.stop();
						track_locations_.remove(item.first);
					}
					//TODO fix errors
				  //else
						//locations_.push_back(track_location);
				}
			}

			//TODO to utils 
			boost::uuids::uuid to_uuid(const DataTypes::Key& key)
			{
				return boost::lexical_cast<boost::uuids::uuid>(key.guid());
			}
						
			std::vector<contracts::locations::ILocation&> locations_;

			concurrency::containers::ConcurrentMap<boost::uuids::uuid, TrackLocation> track_locations_;
		};
	}
}

#endif
