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

			void subscribe(T* observer) override
			{
				if (!has_observer(observer))
					observers_.push_back(observer);
			}

			void unsubscribe(T* observer) override
			{
				if (has_observer(observer))
					observers_.remove(observer);
			}

			bool has_observer(T* observer) override
			{
				return observers_.contains(observer);
			}

			size_t count() const override
			{
				return observers_.size();
			}

			void unsubscribe_all() override
			{
				observers_.clear();
			}			

		protected:
			concurrent::containers::ConcurrentVector<T*> observers_;
		};
	}
}

#endif

