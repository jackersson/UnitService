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

			virtual void subscribe   (T* observer) = 0;
			virtual void unsubscribe (T* observer) = 0;
			virtual bool has_observer(T* observer) = 0;

			virtual void unsubscribe_all() = 0;

			virtual size_t count() const = 0;
		};

		class IObserver
		{
		public:
			virtual ~IObserver() {}

			virtual void on_data() = 0;
		};
	}
}

#endif
