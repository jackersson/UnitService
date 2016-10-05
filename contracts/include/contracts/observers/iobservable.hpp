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

			virtual void subscribe   (const T& observer) = 0;
			virtual void unsubscribe (const T& observer) = 0;
			virtual bool has_observer(const T& observer) = 0;

			virtual void unsubscribe_all() = 0;
		};
	}

	class IObserver
	{

	};
}

#endif
