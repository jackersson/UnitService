#ifndef TrackLocationCoordinator_Included
#define TrackLocationCoordinator_Included

#include <vector>
#include <datatypes/location.pb.h>
#include <concurrent_containers.hpp>
#include <contracts/iunit_context.hpp>

#include <iostream>

#include <boost/uuid/uuid.hpp>

#include "track_location.hpp"
#include <contracts/locations/itrack_location_coordinator.hpp>
#include <contracts/data/data_utils.hpp>

namespace tracking
{
	namespace locations
	{		
		class TrackLocationCoordinator : public contracts::locations::ITrackLocationsCoordinator
			                             , public contracts::observers::IObserver
		                               , public contracts::common::IModule
		{
		public:
			explicit TrackLocationCoordinator(contracts::IUnitContextPtr context)
				: context_(context)

			{}

			void on_data() override
			{
				on_data_changed();
			}

			void init() override {
				location_repository_ = context_->repository()->locations()->local();
				location_repository_->local()->subscribe(this);
			}

			void de_init() override	{
				location_repository_->local()->unsubscribe(this);
			}

			virtual ~TrackLocationCoordinator() {}

			//TODO maybe not need
			const std::vector<contracts::locations::ILocationPtr>& 
				locations() const override {
				return locations_;
			}

			bool grant_access(const DataTypes::Location& location) override
			{
				boost::uuids::uuid uuid;
				if ( !contracts::data::get_guid(location.id(), uuid) )
				{
					context_->logger()->error("Grant access failed. Location not found");
					return false;
				}
				try
				{
					auto it = track_locations_.find(uuid);
					it->access_coordinator().set_state(DataTypes::AccessState::Granted);
				}
				catch (std::exception& exception)	{
					context_->logger()->error("Grant access failed. {0}", exception.what());
					return false;
				}
				
				return true;
			}

		private:
			void on_data_changed()
			{	
				auto& data = *location_repository_->local();
				for (size_t i = 0; i < data.size() ; ++i)
				{
					auto location = data[i];
					boost::uuids::uuid uid;
					if (!contracts::data::get_guid(location.id(), uid))
						continue;
					
					try {
						auto existing = track_locations_.find(uid);
						existing->update(location);
					}
					catch (std::exception& exception)
					{
						std::cout << exception.what() << std::endl;
						auto track_location = std::make_shared<TrackLocation>(location, context_);
						track_locations_.insert(uid, track_location);
					}					
				}

				locations_.clear();
				
				for (auto item : track_locations_)
				{
					auto track_location = item.second;
					auto location = track_location->location();
					if (data.contains(location.id()))
					{
						track_location->stop();
						track_locations_.remove(item.first);
					}
				  else
						locations_.push_back(track_location);
				}
			}
					
			contracts::IUnitContextPtr context_;
			contracts::data::LocationRepositoryPtr location_repository_;
			std::vector<contracts::locations::ILocationPtr> locations_;

			concurrent::containers::ConcurrentMap<boost::uuids::uuid, TrackLocationPtr> track_locations_;
		};
		
	}
}

#endif
