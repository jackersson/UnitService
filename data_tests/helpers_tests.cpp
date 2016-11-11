#include "helpers.hpp"
#include <gtest/gtest.h>

#include <data/models/location.hpp>
#include <data/models/queries.hpp>

namespace repository_tests
{
	Location
		get_any_location(IRepository<Location>* repository)
	{
		std::vector<Location> items;
		auto ok
			= repository->get(GetRequest(GetLocationRequest())
				, items);
		EXPECT_TRUE(ok);
		EXPECT_TRUE(items.size() > 0);
		if (items.size() > 0)
			return items[0];
		return Location();
	}

	Person get_any_person(IRepository<Person>* repository)
	{
		std::vector<Person> items;
		auto ok
			= repository->get(GetRequest(GetPersonRequest())
				, items);
		EXPECT_TRUE(ok);
		EXPECT_TRUE(items.size() > 0);
		return items[0];
	}

	void get_locations_check(IRepository<Location>* repository
		, const GetLocationRequest& request)
	{
		std::vector<Location> items;
		auto ok = repository->get(GetRequest(request), items);
		EXPECT_TRUE(ok);
		EXPECT_TRUE(items.size() > 0);
	}

	void get_persons_check(IRepository<Person>* repository
		, const GetPersonRequest& request)
	{
		std::vector<Person> items;
		auto ok = repository->get(GetRequest(request)
			, items);
		EXPECT_TRUE(ok); //TODO ok not working
		EXPECT_TRUE(items.size() > 0);
	}

	void add_visit_record_check(IRepository<VisitRecord>* repository
		, const VisitRecord& request)
	{
		auto ok = repository->add(request);
		EXPECT_TRUE(ok);
	}


}
