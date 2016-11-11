#ifndef HelpersTests_Included
#define HelpersTests_Included

#include <data/irepository.hpp>

namespace data_model{
	class  VisitRecord;
	class  GetPersonRequest;
	class  GetLocationRequest;
	class  Person ;
	class  Location;
}

using namespace contracts::data;
using namespace data_model;

namespace repository_tests
{
	Location	get_any_location(IRepository<Location>* repository);
	Person  	get_any_person(IRepository<Person>*   repository);

	void get_locations_check(IRepository<Location>* repository
		, const GetLocationRequest& request);

	void get_persons_check(IRepository<Person>* repository
		, const GetPersonRequest& request);

	void add_visit_record_check(IRepository<VisitRecord>* repository
		, const VisitRecord& request);

	template <typename T>
	void check_data_context( AbstractDataContextContainer* container
		                     , bool exists = true)
	{
		auto context = container->get<T>();
		if (exists)
			EXPECT_NE(nullptr, context);
		else
			EXPECT_EQ(nullptr, context);
	}

	template <typename T>
	void check_repository( AbstractRepositoryContainer* container
		                   , bool exists = true)
	{
		auto context = container->get<T>();
		if (exists)
			EXPECT_NE(nullptr, context);
		else
			EXPECT_EQ(nullptr, context);
	}
}

#endif