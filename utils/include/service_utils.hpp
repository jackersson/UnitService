#ifndef ServiceUtils_Included
#define ServiceUtils_Included

#include <future>

namespace utils
{
	namespace service {
		
		template <typename T>
		T get_result(std::promise<T>& promise
			, std::chrono::milliseconds time_duration = std::chrono::milliseconds(50))
		{
			auto future = promise.get_future();
			const auto max_try_count = 10;

			auto try_count = 0;
			while (future.wait_for(time_duration) == std::future_status::timeout
				&& try_count < max_try_count)
				try_count++;

			auto failed = try_count >= max_try_count;
			if (failed)
				throw std::exception("timeout exception");

			return future.get();
		}
	}
}

#endif
