#ifndef Flags_Included
#define Flags_Included
#include <cstdint>

namespace utils
{
	namespace flags
	{
		inline bool has(uint32_t value, uint32_t flag)
		{
			return (value & flag) == flag;
		}

		inline void unset(uint32_t& value, uint32_t flag)
		{
			value = value & ~flag;
		}

		inline void set(uint32_t& value, uint32_t flag)
		{
			value = value | flag;
		}
	}
}

#endif