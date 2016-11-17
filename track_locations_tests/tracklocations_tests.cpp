#include <gtest/gtest.h>
#include <track_locations_engine.hpp>
#include <units/access_device_unit.hpp>
#include "testable_unit_context.hpp"
#include "testable_track_location.hpp"
#include <network_utils.hpp>

#include <data/models/location.hpp>
#include <data/models/devices.hpp>
#include <access_device/access_device_engine.hpp>
#include <data/data_utils.hpp>
#include "testable_configuration.hpp"

using namespace data_model;
using namespace tracking::locations;

namespace track_locations_tests
{
	Location get_location ();
	void     get_locations(std::vector<Location>& items, size_t count);
	Location get_location_with_device();
	
	TEST(TrackLocationTests, TrackLocationsContainerTest)
	{		
		TrackLocationsEngine engine(nullptr);
		engine.init();

		EXPECT_EQ(0, engine.size());

		auto items_count = 2;
		std::vector<Location> locations;
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

	//Connect to controller and watch lights
	TEST(TrackLocationTests, AccessDeviceUnitTest)
	{
		auto device_name = "COM3";

		access_device::AccessDeviceEngine engine;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		
		tracking::units::AccessDeviceObserver  unit(&engine);

		AccessDevice ac(device_name);

		unit.update(ac);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		unit.grant();
		std::this_thread::sleep_for(std::chrono::seconds(5));

		unit  .stop();
		engine.de_init();	
	}

	//API call to update location in container
	TEST(TrackLocationTests, TrackLocationContainerUpdateTest)
	{
		TrackLocationsContainer container;

		auto location_prev = get_location();

		auto track_location =	std::make_shared<TestableTrackLocation>();
		track_location->update(location_prev);
		container.add(track_location);

		auto location_new = get_location();
		location_new.set_name("NewTestable");
		location_new.set_unit_mac_address(location_prev.unit_mac_address());
		location_new.set_id(location_prev.id());

		track_location->update(location_new);

		EXPECT_EQ(location_new, track_location->location());
	}

	TEST(TrackLocationTests, TrackLocationLifecycleTest)
	{
		TestableServiceConfiguration config;
		TestableUnitContext context(&config);
		context.init();
		
		auto track_engine = context.track_locations();

		std::vector<Location> locations;
		locations.push_back(get_location_with_device());
		track_engine->update_with(locations);
		std::this_thread::sleep_for(std::chrono::seconds(5));

		context.de_init();
	}
	
	TEST(TrackLocationTests, GrantAccessTest)
	{
		TestableServiceConfiguration config;
		TestableUnitContext context(&config);
		//context.init();

		auto track_engine = context.track_locations();

		std::vector<Location> locations;
		locations.push_back(get_location_with_device());
		track_engine->update_with(locations);
		std::this_thread::sleep_for(std::chrono::seconds(2));

		track_engine->grant_access(locations[0]);
		std::this_thread::sleep_for(std::chrono::seconds(5));
		context.de_init();
	}
	
	Location get_location()
	{
		Location loc;
		loc.set_name("Testable");
		loc.set_unit_mac_address(utils::network::get_mac_address());
		loc.set_id(contracts::data::get_random_data_key());
		return loc;
	}

	Location get_location_with_device()
	{
		auto loc = get_location();
		AccessDevice access_device(3);
		loc.set_access_device(access_device);
		return loc;
	}

	void get_locations(std::vector<Location>& items, size_t count)
	{
		for (size_t i = 0; i < count; i++)
			items.push_back(get_location());
	}
}
