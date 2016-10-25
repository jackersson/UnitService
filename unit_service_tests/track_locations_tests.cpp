#include <gtest/gtest.h>
#include <track_locations_engine.hpp>
#include "testable_unit_context.hpp"
#include <units/access_device_unit.hpp>


namespace track_locations_tests
{
	DataTypes::Location get_location()
	{
		DataTypes::Location loc;
		loc.set_name("Testable");
		loc.set_allocated_id(new DataTypes::Key(contracts::data::get_random_key()));
		return loc;
	}

	void get_locations(std::vector<DataTypes::Location>& items, size_t count)
	{
		for (auto i = 0; i < count; i++)
			items.push_back(get_location());		
	}

	TEST(TrackLocationsTests, TrackLocationsTests)
	{	
		auto unit_context	= std::make_unique<testable_unit_context::TestableUnitService>();
		tracking::locations::TrackLocationsEngine engine(unit_context.get());
		
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
		EXPECT_TRUE (engine.contains(another_one_location));

		engine.de_init();

		EXPECT_FALSE(false);
	}

	//TODO connect to controller and wathc buttons
	TEST(AccessDeviceUnitTests, AccessDeviceUnitTests)
	{
		auto unit_context = std::make_unique<testable_unit_context::TestableUnitService>();
		unit_context->init();
		tracking::units::AccessDeviceObserver unit(unit_context.get());
		DataTypes::AccessDevice ac;
		ac.set_name("COM3");

		unit.update(ac);

		std::this_thread::sleep_for(std::chrono::seconds(1));

		unit.grant();

		std::this_thread::sleep_for(std::chrono::seconds(5));

		unit.stop();
		unit_context->de_init();
	}
	
	TEST(TrackLocationTest, TrackLocationTests)
	{
		auto unit_context = std::make_unique<testable_unit_context::TestableUnitService>();
		unit_context->init();

		unit_context->de_init();	
	}


	

}
