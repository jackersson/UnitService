#ifndef Rs232ControllerUtils_Included
#define Rs232ControllerUtils_Included
#include <vector>

#include <access_device/rs232/rs232_controller_types.hpp>

namespace access_device
{
	namespace rs232
	{
		const int MIN_BITS_COUNT = 2;
		const int PARTIAL_BITS_SHIFT = 4;
		const int BITS_COUNT_IN_BYTE = 8;
		const int MAX_CHECK_SUM_BITS_COUNT = BITS_COUNT_IN_BYTE;

		const int READ_COMMAND_SIZE = 5;
		const int wRITE_COMMAND_SIZE = 7;

		const int DATA_SIZE = 7;
		const int EMPTY_SIZE = 5;
		const int DALLAS_SIZE = 17; //whole command size
		const int DALLAS_KEY_SIZE = 6; //only data key size

		const int DEVICE_NUMBER_OFFSET = 1;
		const int DATA_OFFSET = 3;

		const int HEADER_INDEX = 0;
		const int PAIR = 2;

		typedef std::pair<unsigned char, unsigned char> bytes_pair;


		inline bool compare(const std::vector<unsigned char>& first
			, const std::vector<unsigned char>& second)
		{
			if (first.size() != second.size())
				return false;

			for (size_t i = 0; i < first.size(); ++i)
			{
				if (first[i] != second[i])
					return false;
			}
			return true;
		}

		inline uint32_t create_bit_mask(uint32_t from, uint32_t to)
		{
			uint32_t r = 0;
			for (auto i = from; i <= to; i++)
				r |= 1 << i;

			return r;
		}

		//TODO may be refactored
		//***
		inline unsigned char word_pair_to_byte(unsigned char a, unsigned char b)
		{
			// 0000 0000 // two parts in 4 bits
			return (a << PARTIAL_BITS_SHIFT) | b;
		}

		inline
			uint16_t byte_pair_to_uint(unsigned char a, unsigned char b)
		{
			// 255 255 // two parts in 8 bits
			return (a << BITS_COUNT_IN_BYTE) | b;
		}

		inline bytes_pair uint_to_word_pair(uint32_t a)
		{
			auto first = create_bit_mask(0, 3);
			auto second = create_bit_mask(4, 7);
			return bytes_pair(((a & second) >> PARTIAL_BITS_SHIFT), (a & first));
		}

		inline bytes_pair uint_to_byte_pair(uint16_t a)
		{
			auto first = create_bit_mask(0, BITS_COUNT_IN_BYTE - 1);
			auto second = create_bit_mask(BITS_COUNT_IN_BYTE, BITS_COUNT_IN_BYTE * 2 - 1);
			return bytes_pair((a & second) >> BITS_COUNT_IN_BYTE, a & first);
		}
		//***


		inline bool check_sum_valid(const std::vector<unsigned char>& bytes)
		{
			auto bytes_size = bytes.size();
			if (bytes_size < MIN_BITS_COUNT)
				return false;

			unsigned short bytes_sum = 0;
			// calculate sum of all received bytes (exept last two - check sum from com port)
			for (size_t i = 0; i < bytes_size - MIN_BITS_COUNT; ++i)
				bytes_sum += bytes[i];

			// checksum can be only 8 bits
			uint32_t mask = create_bit_mask(0, MAX_CHECK_SUM_BITS_COUNT - 1);

			uint32_t check_sum = mask & bytes_sum;

			auto controller_check_sum = word_pair_to_byte(bytes[bytes_size - 2]
				, bytes[bytes_size - 1]);

			return (check_sum == controller_check_sum);
		}

		inline
			std::vector<unsigned char> get_data(const std::vector<unsigned char>& bytes)
		{
			std::vector<unsigned char> result;

			auto estimated_data_count = bytes.size() - PAIR - DATA_OFFSET;
			if (estimated_data_count <= 0 || estimated_data_count % PAIR != 0)
				return result;

			auto j = 0;
			for (size_t i = DATA_OFFSET; i < bytes.size() - PAIR; i += 2, ++j)
				result.push_back(word_pair_to_byte(bytes[i], bytes[i + 1]));

			return result;
		}

		inline unsigned char invert_bits(uint32_t target)
		{
			return ~target & create_bit_mask(0, BITS_COUNT_IN_BYTE - 1);
		}

		inline void invert(std::vector<unsigned char>& bytes)
		{
			for (size_t i = 0; i < bytes.size(); ++i)
				bytes[i] = invert_bits(bytes[i]);
		}

		inline
			bool data_empty(const std::vector<unsigned char>& bytes)
		{
			if (bytes.size() <= 0)
				return true;
			for (auto item : bytes)
			{
				if (item != 0)
					return false;
			}
			return true;
		}

		inline
			void set_header(std::vector<unsigned char>& bytes, port_command header)
		{
			bytes[HEADER_INDEX] = header;
		}

		inline
			void set_device_number(std::vector<unsigned char>& bytes, uint16_t number)
		{
			auto pair = uint_to_byte_pair(number);
			bytes[DEVICE_NUMBER_OFFSET] = pair.first;
			bytes[DEVICE_NUMBER_OFFSET + 1] = pair.second;
		}

		inline
			void set_any_pair(std::vector<unsigned char>& bytes, uint32_t data, int from)
		{
			auto pair = uint_to_word_pair(data);
			bytes[from] = pair.first;
			bytes[from + 1] = pair.second;
		}

		inline
			void set_data(std::vector<unsigned char>& bytes, uint32_t data)
		{
			if (bytes.size() == READ_COMMAND_SIZE)
				return;
			set_any_pair(bytes, data, DATA_OFFSET);
		}

		inline
			bytes_pair check_sum(std::vector<unsigned char>& bytes, int from, int to)
		{
			auto sum = 0;
			for (auto i = from; i < to; i++)
				sum += bytes[i];
			return uint_to_word_pair(sum);
		}

		inline
			void set_check_sum(std::vector<unsigned char>& bytes)
		{
			auto count = bytes.size();
			auto pair = check_sum(bytes, 0, count - PAIR);
			bytes[count - PAIR] = pair.first;
			bytes[count - 1] = pair.second;
		}

		inline 
			void set_data_key( std::vector<unsigned char>& command
				               , const std::vector<unsigned char>& key
				               , int start_position)
		{
			if (command.size() < key.size() * PAIR)
				return;
			for (auto num : key)
			{
				set_any_pair(command, num, start_position);
				start_position += PAIR;
			}
		}


		inline
			void update_command(std::vector<unsigned char>& bytes
				, uint16_t device_number
				, uint32_t data)
		{
			set_device_number(bytes, device_number);
			set_data(bytes, data);
			set_check_sum(bytes);
		}

		inline
			unsigned char get_header(const std::vector<unsigned char>& bytes)
		{
			if (bytes.size() <= HEADER_INDEX)
				return 0;

			return bytes[HEADER_INDEX];
		}

		inline 
			void create_data_command ( std::vector<unsigned char>& command
				                       , port_command id
				                       , uint32_t data
				                       , uint16_t device_number	)
		{
			if (command.size() < DATA_SIZE)
				command.resize(DATA_SIZE);

			set_header       (command, id);
			set_device_number(command, device_number);
			set_data         (command, data);
			set_check_sum    (command);
		}

		inline
			void create_empty_command(std::vector<unsigned char>& command
				, port_command id		
				, uint16_t device_number)
		{
			if (command.size() < READ_COMMAND_SIZE)
				command.resize(READ_COMMAND_SIZE);

			set_header       (command, id);
			set_device_number(command, device_number);
			set_check_sum    (command);
		}

		inline
			void create_data_key_command(std::vector<unsigned char>& command
				, const std::vector<unsigned char>& key
				, port_command id
				, uint16_t device_number)
		{
			auto total_size = DATA_SIZE + key.size() * PAIR;
			if (command.size() < total_size)
				command.resize(total_size);

			set_header       (command, id);
			set_device_number(command, device_number);
			set_data_key     (command, key, DATA_OFFSET);
			set_check_sum    (command);
		}
	}
}

#endif
