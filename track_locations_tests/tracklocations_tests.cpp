#include <gtest/gtest.h>
#include <track_locations_engine.hpp>
#include <units/access_device_unit.hpp>

namespace track_locations_tests
{
	DataTypes::Location get_location();
	void get_locations(std::vector<DataTypes::Location>& items, size_t count);	
	
	TEST(TrackLocationTests, TrackLocationsContainerTest)
	{		
		tracking::locations::TrackLocationsEngine engine(nullptr);
		engine.init();

		EXPECT_EQ(0, engine.size());

		auto items_count = 2;
		std::vector<DataTypes::Location> locations;
		get_locations(locations, items_count);

		//accepts vector and initialize it
		engine.update_with(locations);
		EXPECT_EQ(items_count, engine.size());
		EXPECT_TRUE(engine.contains(locations[0]));

		//accepts custom locations and check if contains work
		auto another_one_location = get_location();
		EXPECT_FALSE(engine.contains(another_one_location));

		//accepts more locations and check size
		locations.push_back(another_one_location);
		engine.update_with(locations);
		EXPECT_EQ(items_count + 1, engine.size());
		EXPECT_TRUE(engine.contains(another_one_location));

		//accepts less locations and check size
		locations.clear();
		locations.push_back(another_one_location);
		engine.update_with(locations);
		EXPECT_EQ(items_count - 1, engine.size());
		EXPECT_TRUE(engine.contains(another_one_location));

		engine.de_init();

		EXPECT_FALSE(false);
	}

	//TODO connect to controller and watch buttons
	TEST(TrackLocationTests, AccessDeviceUnitTest)
	{
		auto device_name = "COM3";

		tracking::units::AccessDeviceUnitContext context;
		tracking::units::AccessDeviceObserver unit(context);
		DataTypes::AccessDevice ac;
		ac.set_name(device_name);

		unit.update(ac);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		unit.grant();
		std::this_thread::sleep_for(std::chrono::seconds(5));

		unit.stop();
	}

	TEST(TrackLocationTests, TrackLocationTest)
	{

	}

	DataTypes::Location get_location()
	{
		DataTypes::Location loc;
		loc.set_name("Testable");
		loc.set_allocated_id(new DataTypes::Key(contracts::data::get_random_key()));
		return loc;
	}

	void get_locations(std::vector<DataTypes::Location>& items, size_t count)
	{
		for (size_t i = 0; i < count; i++)
			items.push_back(get_location());
	}




}
