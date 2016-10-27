#include <gtest/gtest.h>
#include <datatypes/location.pb.h>
#include <contracts/data/irepository.hpp>
#include <datatypes/queries.pb.h>
#include <repository_container.hpp>

#include <database_service/database_client_impl.hpp>
#include <datacontext_container.hpp>
#include "testable_database_impl.hpp"

using namespace contracts::data;

namespace repository_tests
{
	DataTypes::Location
		get_any_location(IRepository<DataTypes::Location>* repository);

	DataTypes::Person
		get_any_person(IRepository<DataTypes::Person>* repository);

	void get_locations_check(IRepository<DataTypes::Location>* repository
		                       , DataTypes::GetLocationRequest* request);

	void get_persons_check(IRepository<DataTypes::Person>* repository
		                    , DataTypes::GetPersonRequest* request);

	void add_visit_record_check(IRepository<DataTypes::VisitRecord>* repository
	                           , DataTypes::VisitRecord* request);

	template <typename T>
	void check_data_context( AbstractDataContextContainer* container
	                       , bool exists = true);

	template <typename T>
	void check_repository(AbstractRepositoryContainer* container, bool exists = true);


	//Data
	TEST(RepositoryTests, DataContextContainerTest)
	{
		data_core::DataContextContainer container(nullptr);

		container.init();

		check_data_context<DataTypes::Location>   (&container);
		check_data_context<DataTypes::VisitRecord>(&container);
		check_data_context<DataTypes::Person>     (&container);
		check_data_context<DataTypes::Card>       (&container, false);
		
		container.de_init();
	}
	
	TEST(RepositoryTests, RepositoryContainerTest)
	{
		data_core::RepositoryContainer container;

		container.init();

		check_repository<DataTypes::Location>   (&container);
		check_repository<DataTypes::VisitRecord>(&container);
		check_repository<DataTypes::Person>     (&container);
		check_repository<DataTypes::Card>       (&container, false);

		container.de_init();
	}

	TEST(RepositoryTests, GetLocationTest)
	{
		data_tests::TestableDatabaseImpl impl("127.0.0.1:49065");
		auto repository = impl.get<DataTypes::Location>();
		
		//Should return all locations
		auto all_request = new DataTypes::GetLocationRequest();
		get_locations_check(repository, all_request);


		//Should return only locations with specified mac_address
		auto macaddress_request = new DataTypes::GetLocationRequest();
		macaddress_request->set_allocated_mac_address(
			                         new std::string("Mac636125689415206209"));

		get_locations_check(repository, macaddress_request);

		impl.stop();
	}

	TEST(RepositoryTests, GetPersonTest)
	{
		data_tests::TestableDatabaseImpl impl("127.0.0.1:49065");
		auto repository = impl.get<DataTypes::Person>();		

		auto all_request = new DataTypes::GetPersonRequest();
		all_request->set_card("636115413092286900");
		get_persons_check(repository, all_request);

		impl.stop();
	}

	TEST(RepositoryTests, AddVisitorTest)
	{
		data_tests::TestableDatabaseImpl impl("127.0.0.1:49065");
		auto locations_repository = impl.get<DataTypes::Location>();		
		
		auto existingLocation = get_any_location(locations_repository);

		//TODO move from here
		auto visit_record = new DataTypes::VisitRecord();
		auto key = new DataTypes::Key(get_random_key());
		visit_record->set_allocated_id(key);
		visit_record->set_allocated_time(get_current_time());
		visit_record->set_state(DataTypes::AccessState::Granted);
		visit_record->set_allocated_location_id(new DataTypes::Key(existingLocation.id()));
		auto card = new DataTypes::Card();
		auto card_key = new DataTypes::Key();
		card_key->set_identifier("1234 6563 3455 34553");
		card->set_allocated_unique_identifier(card_key);
		visit_record->set_allocated_card(card);
		//*************************

		auto visitrecords_repository = impl.get<DataTypes::VisitRecord>();
		add_visit_record_check(visitrecords_repository, visit_record);

		impl.stop();
	}


	DataTypes::Location
		get_any_location(contracts::data::IRepository<DataTypes::Location>* repository)
	{
		auto request = new DataTypes::GetLocationRequest();
		std::vector<DataTypes::Location> items;
		auto ok = repository->get(request, items);
		EXPECT_TRUE(ok);
		EXPECT_TRUE(items.size() > 0);
		return items[0];
	}

	DataTypes::Person
		get_any_person(IRepository<DataTypes::Person>* repository)
	{
		auto request = new DataTypes::GetPersonRequest();
		std::vector<DataTypes::Person> items;
		auto ok = repository->get(request, items);
		EXPECT_TRUE(ok);
		EXPECT_TRUE(items.size() > 0);
		return items[0];
	}

	void get_locations_check(IRepository<DataTypes::Location>* repository
		, DataTypes::GetLocationRequest* request)
	{
		std::vector<DataTypes::Location> items;
		auto ok = repository->get(request, items);
		EXPECT_TRUE(ok);
		EXPECT_TRUE(items.size() > 0);
	}

	void get_persons_check(IRepository<DataTypes::Person>* repository
		, DataTypes::GetPersonRequest* request)
	{
		std::vector<DataTypes::Person> items;
		auto ok = repository->get(request, items);
		EXPECT_TRUE(ok); //TODO ok not working
		EXPECT_TRUE(items.size() > 0);
	}

	void add_visit_record_check(IRepository<DataTypes::VisitRecord>* repository
		, DataTypes::VisitRecord* request)
	{
		auto ok = repository->add(request);
		EXPECT_TRUE(ok);
	}

	template <typename T>
	void check_data_context( AbstractDataContextContainer* container
	                       , bool exists)
	{
		auto context = container->get<T>();
		if  (exists)
		  EXPECT_NE(nullptr, context);
		else 
			EXPECT_EQ(nullptr, context);
	}

	template <typename T>
	void check_repository(AbstractRepositoryContainer* container, bool exists)
	{
		auto context = container->get<T>();
		if (exists)
			EXPECT_NE(nullptr, context);
		else
			EXPECT_EQ(nullptr, context);
	}


}
