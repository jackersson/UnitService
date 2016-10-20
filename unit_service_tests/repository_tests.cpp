#include <gtest/gtest.h>
#include <launcher.hpp>

namespace repository_tests
{	

	DataTypes::Location 
		get_any_location(contracts::data::LocationRepositoryPtr repository)
	{		
		auto request = new DataTypes::GetLocationRequest();
		std::vector<DataTypes::Location> items;
		auto ok = repository->get(request, items);
		EXPECT_TRUE(ok);
		EXPECT_TRUE(items.size() > 0);
		return items[0];
	}

	DataTypes::Person
		get_any_person(contracts::data::PersonRepositoryPtr repository)
	{
		auto request = new DataTypes::GetPersonRequest();
		std::vector<DataTypes::Person> items;
		auto ok = repository->get(request, items);
		EXPECT_TRUE(ok);
		EXPECT_TRUE(items.size() > 0);
		return items[0];
	}

	void get_locations_check(	contracts::data::LocationRepositoryPtr repository
	                      	, DataTypes::GetLocationRequest* request)
	{
		std::vector<DataTypes::Location> items;
		auto ok = repository->get(request, items);
		EXPECT_TRUE(ok);
		EXPECT_TRUE(items.size() > 0);
	}

	void get_persons_check( contracts::data::PersonRepositoryPtr repository
		                    , DataTypes::GetPersonRequest* request)
	{
		std::vector<DataTypes::Person> items;
		auto ok = repository->get(request, items);
		EXPECT_TRUE(ok); //TODO ok not working
		EXPECT_TRUE(items.size() > 0);
	}

	void add_visit_record_check( contracts::data::VisitRecordRepositoryPtr repository
		                         , DataTypes::VisitRecord* request)
	{
		auto ok = repository->add(request);
		EXPECT_TRUE(ok);
	}

	//Data

	TEST(GetLocationTests, GetLocationTests)
	{
		Launcher launcher;
		launcher.init();

		auto repository = launcher.context().repository()->locations();
		
		//Should return all locations
		auto all_request = new DataTypes::GetLocationRequest();
		get_locations_check(repository, all_request);

		//Should return only locations with specified mac_address
		auto macaddress_request = new DataTypes::GetLocationRequest();
		macaddress_request->set_allocated_mac_address(
			                  new std::string("Mac636125689415206209"));

		get_locations_check(repository, macaddress_request);	
	}

	TEST(GetPersonTest, GetPersonTest)
	{
		Launcher launcher;
		launcher.init();
				
		auto repository = launcher.context().repository()->persons();

		auto all_request = new DataTypes::GetPersonRequest();
		all_request->set_card("636115413092286900");
		get_persons_check(repository, all_request);		
	}

	TEST(AddVisitorTest, AddVisitorTest)
	{
		Launcher launcher;
		launcher.init();

		auto locations_repository = launcher.context().repository()->locations();
		
		auto existingLocation = get_any_location(locations_repository);

		auto visit_record = new DataTypes::VisitRecord();
		
		auto key = new DataTypes::Key(contracts::data::get_random_key());
		visit_record->set_allocated_id(key);

		visit_record->set_allocated_time(contracts::data::get_current_time());
		visit_record->set_state(DataTypes::AccessState::Granted);
		visit_record->set_allocated_location_id(new DataTypes::Key(existingLocation.id()));
		auto card = new DataTypes::Card();
		auto card_key = new DataTypes::Key();
		card_key->set_identifier("1234 6563 3455 34553");
		card->set_allocated_unique_identifier(card_key);
		visit_record->set_allocated_card(card);
		
		auto repository = launcher.context().repository()->visit_records();

		add_visit_record_check(repository, visit_record);
	}

}
