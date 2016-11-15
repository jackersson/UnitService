#ifndef SimpleThreadable_Included
#define SimpleThreadable_Included
#include <threadable.hpp>

namespace utils
{
	class SimpleThreadable : public Threadable
	{
	public:
		SimpleThreadable() {}

	protected:
		void run() override
		{
			while (active())
			{
				std::cout << "do" << std::endl;
				if (cancelation_requested)
					break;
			}
			std::cout << "done" << std::endl;
		}
	};
}

#endif