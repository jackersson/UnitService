#ifndef Observable_Included
#define Observable_Included

#include "iobservable.hpp"
#include <contracts/common/iequatable.hpp>
#include <concurrent_containers.hpp>


namespace contracts
{
	namespace observers
	{
		template <typename T>
		class Observable : public IObservable<T>
		{
		public:
			virtual ~Observable() {}

			void subscribe(const T& observer) override
			{
				common::IObject& obj = observer;
				if (!has_observer(observer))
					observers_.insert(obj.get_hash_code(), observer);
			}

			void unsubscribe(const T& observer) override
			{
				common::IObject& obj = observer;
				if (has_observer(observer))
					observers_.remove(obj.get_hash_code());
			}

			bool has_observer(const T& observer) override
			{
				common::IObject& obj = observer;
				return observers_.contains(obj.get_hash_code());
			}

			void unsubscribe_all() override
			{
				observers_.clear();
			}

		private:
			concurrency::containers::ConcurrentMap<int, T> observers_;
		};
	}
}

#endif

