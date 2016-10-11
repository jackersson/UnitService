#ifndef IObservable_Included
#define IObservable_Included


namespace contracts
{
	namespace observers
	{
		template <typename T>
		class IObservable
		{
		public:
			virtual ~IObservable() {}

			virtual void subscribe   (std::shared_ptr<T> observer) = 0;
			virtual void unsubscribe (std::shared_ptr<T> observer) = 0;
			virtual bool has_observer(std::shared_ptr<T> observer) = 0;

			virtual void unsubscribe_all() = 0;
		};
	}

	class IObserver
	{

	};
}

#endif
