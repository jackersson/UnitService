#include <gtest/gtest.h>
#include <data/models/key.hpp>
#include <data/data_utils.hpp>

namespace data_key_tests
{
	TEST(DataKeyTests, OperatorsTest)
	{
		auto first  = contracts::data::get_random_data_key();
		auto second = contracts::data::get_random_data_key();

		auto fmores = first  > second;
		auto smoref = second > first ;

		EXPECT_NE(fmores, smoref);
	}

	TEST(DataKeyTests, InsertInSetTest)
	{
		auto total_count = 10;
		std::set<data_model::Key> target_locations_set;

		for (auto i = 0; i < total_count; ++i)
		{
			auto new_key = contracts::data::get_random_data_key();
			EXPECT_EQ(target_locations_set.find(new_key), target_locations_set.end());
			
			target_locations_set.insert(new_key);
		}

		EXPECT_EQ(total_count, target_locations_set.size());
	}
}
