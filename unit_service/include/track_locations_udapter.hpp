#ifndef TrackLocationsUpdater_Included
#define TrackLocationsUpdater_Included

#include <vector>
#include <datatypes/location.pb.h>
#include <contracts/iunit_context.hpp>
#include <observers/iobservable.hpp>
#include <data/irepository.hpp>
#include <contracts/locations/itrack_location_coordinator.hpp>

class TrackLocationsUpdater : public contracts::observers::IObserver
	                          , public contracts::common::IModule
{
public:
	explicit TrackLocationsUpdater( contracts::IUnitContext* context)
		                            : context_(context)
		                            , location_repository_(nullptr)
		                            , track_locations_(nullptr)
	{}

	virtual ~TrackLocationsUpdater()	{
		TrackLocationsUpdater::de_init();
	}			

	void init() override {
		track_locations_     = context_->track_locations();
		location_repository_ = context_->repository()->get<DataTypes::Location>();
		location_repository_->local()->subscribe(this);
	}

	void de_init() override {
		if (location_repository_ == nullptr)
			return;
		auto local = location_repository_->local();
		if (local == nullptr)
			return;
		local->unsubscribe(this);
	}
	
	void on_data() override
	{
		auto& data = *location_repository_->local();
		std::vector<DataTypes::Location> locations;
		for (size_t i = 0; i < data.size(); ++i)
			locations.push_back(data[i]);
		track_locations_->update_with(locations);
	}

private:
	contracts::IUnitContext* context_;
	contracts::data::IRepository<DataTypes::Location>* location_repository_;
	contracts::locations::ITrackLocationsEngine* track_locations_;
};


#endif
