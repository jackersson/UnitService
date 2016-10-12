#ifndef Observable_Included
#define Observable_Included

#include "iobservable.hpp"
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

			void subscribe(std::shared_ptr<T> observer) override
			{
				if (!has_observer(observer))
					observers_.push_back(observer);
			}

			void unsubscribe(std::shared_ptr<T> observer) override
			{
				if (has_observer(observer))
					observers_.remove(observer);
			}

			bool has_observer(std::shared_ptr<T> observer) override
			{
				return observers_.contains(observer);
			}

			void unsubscribe_all() override
			{
				observers_.clear();
			}			

		protected:
			concurrent::containers::ConcurrentVector<std::shared_ptr<T>> observers_;
		};
	}
}

#endif

