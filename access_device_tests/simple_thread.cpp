#include "simple_threadable.hpp"

namespace utils
{	
	SimpleThreadable::SimpleThreadable() {}

	SimpleThreadable::~SimpleThreadable() {}

	void SimpleThreadable::run() 
	{
		while (active())
		{
			std::cout << "do" << std::endl;
			if (cancelation_requested)
				break;
		}
		std::cout << "done" << std::endl;
	}	
}
