#ifndef TrackLocationsUpdater_Included
#define TrackLocationsUpdater_Included

#include <vector>
#include <datatypes/location.pb.h>
#include <contracts/iunit_context.hpp>

class TrackLocationsUpdater : public contracts::observers::IObserver
	                          , public contracts::common::IModule
{
public:
	explicit TrackLocationsUpdater(contracts::IUnitContext* context)
		: context_(context)
	{
		track_locations_ = context->track_locations();
	}

	virtual ~TrackLocationsUpdater() {}			

	void init() override {
		location_repository_ = context_->repository()->locations();
		location_repository_->local()->subscribe(this);
	}

	void de_init() override {
		location_repository_->local()->unsubscribe(this);
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
	contracts::data::LocationRepositoryPtr location_repository_;
	contracts::locations::ITrackLocationsEnginePtr track_locations_;
};


#endif
