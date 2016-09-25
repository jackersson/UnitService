#include <gtest/gtest.h>
#include <iexecutable_command.hpp>
#include <rs232_controller_utils.hpp>

using namespace rs323_controller;

namespace rs232_controller_tests
{
	void word_pair_check(port_command command)
	{
		auto pair   = uint_to_word_pair(command);
		auto output = word_pair_to_byte(pair.first, pair.second);

		EXPECT_EQ(command, output);
	}

	void invert_bits_check(uint32_t value, uint32_t expected)
	{
		auto result = invert_bits(value);
		EXPECT_EQ(expected, result);
	}

	TEST(WordPairTest, WordPairToByteConversionTest)
	{
		word_pair_check(ReadB);
		word_pair_check(WriteB);
		word_pair_check(Dallas);
		word_pair_check(Master);
		word_pair_check(ReadC);
	}

	TEST(InvertBitsTest, InvertBitsTest)
	{
		invert_bits_check(239, 16 );
		invert_bits_check(1  , 254);
	}

}
