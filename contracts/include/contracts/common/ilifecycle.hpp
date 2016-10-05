#ifndef ILifecycle_Included
#define ILifecycle_Included

namespace contracts
{
	namespace common
	{
		class ILifecycle
		{
		public:
			virtual ~ILifecycle(){}

			virtual void start() = 0;
			virtual void stop () = 0;
		};

		class IModule
		{
		public:
			virtual ~IModule() {}

			virtual void init()    = 0;
			virtual void de_init() = 0;

		};
	}
}

#endif
