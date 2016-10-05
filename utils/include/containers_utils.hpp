#ifndef ContainersUtils_Included
#define ContainersUtils_Included
#include <vector>


namespace utils
{
	namespace containers {
		template<typename T>
		bool contains(const std::vector<T>& container, T value)
		{
			for (auto it : container)
			{
				if (it == value)
					return true;
			}
			return false;
		}
	}
}

#endif
