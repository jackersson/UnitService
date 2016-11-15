#include <gtest/gtest.h>
#include <access_device/rs232/rs232_controller_types.hpp>
#include <access_device/rs232/rs232_controller_utils.hpp>
#include <access_device/core/iexecutable_command.hpp>
#include "test_command_context.hpp"
#include <flags.hpp>
//#include <access_device/access_device_engine.hpp>

using namespace access_device::rs232   ;
using namespace access_device::commands;
using namespace access_device::core    ;


namespace access_device_tests
{
	void word_pair_check(port_command command)
	{
		auto pair = uint_to_word_pair(command);
		auto output = word_pair_to_byte(pair.first, pair.second);

		EXPECT_EQ(command, output);
	}

	void byte_pair_check(uint16_t input)
	{
		auto pair = uint_to_byte_pair(input);
		auto output = byte_pair_to_uint(pair.first, pair.second);
		EXPECT_EQ(input, output);
	}

	void invert_bits_check(uint32_t value, uint32_t expected)
	{
		auto result = invert_bits(value);
		EXPECT_EQ(expected, result);
	}

	void or_bits_check(uint32_t first, uint32_t second, uint32_t expected)
	{
		auto result = first | second;
		EXPECT_EQ(expected, result);
	}

	void create_read_command_check(port_command command)
	{
		std::vector<unsigned char> bytes;
		create_empty_command(bytes, command, 0);
		EXPECT_TRUE(check_sum_valid(bytes));
	}

	void create_data_key_command_check(const std::vector<unsigned char>& key)
	{
		std::vector<unsigned char> bytes;
		create_data_key_command(bytes, key, port_command::Dallas, 0);
		EXPECT_TRUE(check_sum_valid(bytes));
	}
	
	//with controller
	void light_command_check( TestCommandContext&   test_cmd
		                      , IExecutableCommandPtr command
		                      , uint32_t id
		                      , bool with_data = true)
	{
		//Not need to set data when ask buttons
		if (with_data)
			command->command().set_data(id);
		try
		{
			auto result = test_cmd.execute(command);
		
			EXPECT_TRUE(result->is_valid());
			EXPECT_EQ  (id, result->small_data());
		}
		catch(std::exception& ex)
		{
			EXPECT_EQ("", ex.what());
		}
	}
	

	enum FlagsTest
	{
		UnsetFlags = 0
		, Flag1 = 1
		, Flag2 = 2
	};

	enum FlagsWithShiftTest
	{
		  UnsetFlagsWithShift = 0
		, FlagsWithShift1 = 1 << 1
		, FlagsWithShift2 = 1 << 2
		, FlagsWithShift3 = 1 << 4
		, FlagsWithShift4 = 1 << 8
	};

	void check_flags()
	{
		uint32_t val = 0;
		EXPECT_FALSE(utils::flags::has(val, Flag1));
		EXPECT_FALSE(utils::flags::has(val, Flag2));

		utils::flags::set(val, Flag1);
		EXPECT_TRUE (utils::flags::has(val, Flag1));
		EXPECT_FALSE(utils::flags::has(val, Flag2));

		utils::flags::set(val, Flag2);
		EXPECT_TRUE(utils::flags::has(val, Flag1));
		EXPECT_TRUE(utils::flags::has(val, Flag2));
	}

	void check_flags_with_shift()
	{
		uint32_t val = 0;
		EXPECT_FALSE(utils::flags::has(val, FlagsWithShift1));
		EXPECT_FALSE(utils::flags::has(val, FlagsWithShift2));
		EXPECT_FALSE(utils::flags::has(val, FlagsWithShift3));

		utils::flags::set(val, FlagsWithShift1);
		EXPECT_TRUE (utils::flags::has(val, FlagsWithShift1));
		EXPECT_FALSE(utils::flags::has(val, FlagsWithShift2));
		EXPECT_FALSE(utils::flags::has(val, FlagsWithShift3));

		utils::flags::set(val, FlagsWithShift2);
		EXPECT_TRUE (utils::flags::has(val, FlagsWithShift1));
		EXPECT_TRUE (utils::flags::has(val, FlagsWithShift2));
		EXPECT_FALSE(utils::flags::has(val, FlagsWithShift3));

		utils::flags::set(val, FlagsWithShift3);
		EXPECT_TRUE(utils::flags::has(val, FlagsWithShift1));
		EXPECT_TRUE(utils::flags::has(val, FlagsWithShift2));
		EXPECT_TRUE(utils::flags::has(val, FlagsWithShift3));
	}

	TEST(WordPairTest, WordPairToByteConversionTest)
	{
		word_pair_check(ReadB);
		word_pair_check(WriteB);
		word_pair_check(port_command::Dallas);
		word_pair_check(Master);
		word_pair_check(ReadC);
	}

	TEST(InvertBitsTest, InvertBitsTest)
	{
		invert_bits_check(239, 16);
		invert_bits_check(1, 254);
	}

	TEST(BytePairTest, BytePairToShortConversionTest)
	{
		byte_pair_check(1024);
	}

	TEST(CreateReadCommandTest, CreateReadCommandTest)
	{
		create_read_command_check(ReadC);
	}

	TEST(OrBitsTest, OrBitsTest)
	{
		or_bits_check(2, 16, 18);
		or_bits_check(Pc1Access, Pc4OpenGate, 18);
	}

	TEST(CreateDataKeyCommandTest, CreateDataKeyCommandTest)
	{
		std::vector<unsigned char> key = { 255, 255, 255, 255, 255, 255 };
		create_data_key_command_check(key);
	}


	TEST(FlagsTestGoogle, FlagsTestGoogle)
	{
		check_flags();
		check_flags_with_shift();
	}

	//with controller
	TEST(LightCommandTest, LightCommandTest)
	{
		CommandFactory factory;
		auto command = factory.get<LightCommand>();
		//TODO check first com port name before running test
		TestCommandContext cm_test("COM6");

		std::vector<uint32_t> examples = 
		{   
			  Pb1RedMain | Pb6Green
			, 0
			, Pb1RedMain | Pb6Green | Pb8GreenAccess
			, 0 
		};

		for (auto num : examples)
			light_command_check(cm_test, command, num);
	}

	TEST(ButtonCommandTest, ButtonCommandTest)
	{
		CommandFactory factory;
		auto command = factory.get<ButtonCommand>();
		//TODO check first com port name before running test
		TestCommandContext cm_test("COM3");

		//Press Both buttons to pass the following
		std::vector<uint32_t> examples = { Pc1Access | Pc4OpenGate };

		for (auto num : examples)
			light_command_check(cm_test, command, num, false);
	}

	TEST(DataKeyCommandTest, DataKeyCommandTest)
	{
		CommandFactory factory;
		auto command = factory.get<DallasCommand>();
		//TODO check first com port name before running test
		TestCommandContext cm_test("COM3");

		//Put dallas key to pass the following
		auto result = cm_test.execute(command);

		EXPECT_TRUE (result->is_valid());
		EXPECT_FALSE(result->is_empty());	
		EXPECT_EQ   (DALLAS_KEY_SIZE, result->full_data().size());
	}

	TEST(AccessDeviceCommandTest, DeviceInfoCommandTest)
	{
		CommandFactory factory;
		auto command = factory.get<DeviceInfoCommand>();
		//TODO check first com port name before running test
		TestCommandContext cm_test("COM6");		
		
		//Put device with not 0 id
		auto result = cm_test.execute(command);

		EXPECT_TRUE (result->is_valid());
		EXPECT_TRUE (result->device_number() > 0);

	}

	TEST(AccessDeviceCommandTest, CommandInfoParseTest)
	{
		std::vector<unsigned char> cmd = { 129, 0, 3, 0, 0, 8, 4 };

		//CommandInfo cinfo;
		//cinfo.parse(cmd);

		//EXPECT_TRUE(129, cinfo.header);
		//EXPECT_TRUE(3  , cinfo.device_number);
		//EXPECT_TRUE(1  , cinfo.data.size()  );
		//EXPECT_TRUE(cinfo.valid);
	}




}
