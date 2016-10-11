#ifndef TrackLocationCoordinator_Included
#define TrackLocationCoordinator_Included
#include <vector>
#include <datatypes/location.pb.h>
#include <concurrent_containers.hpp>
#include <contracts/iunit_context.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
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
			explicit TrackLocationCoordinator(contracts::IUnitContextPtr context)
				: context_(context)
			{
				auto locations = context_->repository()->locations()->local();
				locations->subscribe(
					std::bind(&TrackLocationCoordinator::on_data_changed, this));
			}

			virtual ~TrackLocationCoordinator() {}

		private:
			void on_data_changed()
			{
			//	auto locations = context_->repository()->locations();
			//	auto data = locations->local()->entities(); //TODO implement
				//TODO check with current mac address
				std::vector<DataTypes::Location> data;

				for (auto location : data)
				{
					auto guid = to_uuid(location.id());
					
					try {
						auto existing = track_locations_.find(guid);
						existing->update(location);
					}
					catch (std::exception& exception)
					{
						auto track_location = std::make_shared<TrackLocation>(location, context_);
						track_locations_.insert(guid, track_location);
					}					
				}

				locations_.clear();
				
				for (auto item : track_locations_)
				{
					auto track_location = item.second;
					auto location = track_location->location();
					if (contains(data, location))
					{
						track_location->stop();
						track_locations_.remove(item.first);
					}
					//TODO fix errors
				  //else
						//locations_.push_back(track_location);
				}
			}

			static bool contains( const std::vector<DataTypes::Location>& container
				                  , DataTypes::Location value)
			{				
				for (auto it : container)
				{
					if (it.id().guid() == value.id().guid())
						return true;
				}
				return false;
			}
		

			//TODO to utils 
			boost::uuids::uuid to_uuid(const DataTypes::Key& key)
			{
				return boost::uuids::random_generator()();
				//TODO check why its not working
				//return boost::lexical_cast<boost::uuids::uuid>(key.guid());
			}

			contracts::IUnitContextPtr context_;

			std::vector<contracts::locations::ILocationPtr> locations_;


			concurrency::containers::ConcurrentMap<boost::uuids::uuid, TrackLocationPtr> track_locations_;
		};
		
	}
}

#endif
