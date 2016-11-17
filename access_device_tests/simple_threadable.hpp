#ifndef SimpleThreadable_Included
#define SimpleThreadable_Included
#include <threadable.hpp>

namespace utils
{
	class SimpleThreadable : public Threadable
	{
	public:
		SimpleThreadable();
		~SimpleThreadable();
	protected:
		void run() override;
	};
}

#endif 

