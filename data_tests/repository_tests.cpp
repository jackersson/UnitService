#include <gtest/gtest.h>
#include <data/irepository.hpp>

#include <database_service/database_client_data_api.hpp>
#include "testable_database_impl.hpp"
#include <database_service/datacontext/datacontext_container.hpp>
#include <repository_container.hpp>
#include "helpers.hpp"
#include <data/models/card.hpp>
#include <data/models/queries.hpp>

namespace repository_tests
{	
	TEST(RepositoryTests, DataContextContainerTest)
	{
		services_api::datacontext::DataContextContainer container(nullptr);

		container.init();

		check_data_context<Location>   (&container);
		check_data_context<VisitRecord>(&container);
		check_data_context<Person>     (&container);
		check_data_context<Card>       (&container, false);
		
		container.de_init();
	}
	
	TEST(RepositoryTests, RepositoryContainerTest)
	{
		data_core::RepositoryContainer container(nullptr);

		container.init();

		check_repository<Location>   (&container);
		check_repository<VisitRecord>(&container);
		check_repository<Person>     (&container);
		check_repository<Card>       (&container, false);

		container.de_init();
	}

	TEST(RepositoryTests, GetLocationTest)
	{
		data_tests::TestableDatabaseImpl impl("127.0.0.1:49065");
		auto repository = impl.get<Location>();

		//Should return all locations
		GetLocationRequest all_request;
		get_locations_check(repository, all_request);

		//Should return only locations with specified mac_address
		GetLocationRequest macaddress_request;
		macaddress_request.set_macaddress("Mac636143845822343075");
		get_locations_check(repository, macaddress_request);

		impl.stop();
	}

	TEST(RepositoryTests, GetPersonTest)
	{
		data_tests::TestableDatabaseImpl impl("127.0.0.1:49065");
		auto repository = impl.get<Person>();		

		GetPersonRequest all_request;
		all_request.set_card(Card(Key("636143939418496846")));
		get_persons_check(repository, all_request);

		impl.stop();
	}

	TEST(RepositoryTests, AddVisitorTest)
	{
		data_tests::TestableDatabaseImpl impl("127.0.0.1:49065");
		auto locations_repository = impl.get<Location>();		
		
		auto existingLocation = get_any_location(locations_repository);

		//TODO move from here
		VisitRecord visit_record;
		visit_record.set_now();
		visit_record.set_access_state(Granted);
		visit_record.set_location_id(existingLocation.id());

		Card card(Key("1234 6563 3455 34553"));
		card.set_owner_id(get_random_data_key());
		visit_record.set_card(card);
		//*************************

		auto visitrecords_repository = impl.get<VisitRecord>();
		add_visit_record_check(visitrecords_repository, visit_record);
		impl.stop();
	}
}
