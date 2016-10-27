#include <gtest/gtest.h>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <localstorage/locations_localstorage.hpp>

namespace data_utils_tests
{	
	TEST(DataUtilsTests, GuidParserTests)
	{
		DataTypes::Key key;
		auto uud = boost::uuids::random_generator()();
		contracts::data::set_guid(uud, key);

		boost::uuids::uuid parsed_guid;
		auto result = contracts::data::get_guid(key, parsed_guid);

		EXPECT_TRUE(result);
		EXPECT_EQ(uud, parsed_guid);

		auto uu1 = to_string(uud);
		auto uu2 = to_string(parsed_guid);
		EXPECT_EQ(uu1, uu2);
	}
}
