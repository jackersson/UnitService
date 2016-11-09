#include <string>
#include <iostream>
#include "launcher.hpp"


void init()
{
	Launcher launcher;
	launcher.init();
	std::cin.get();
}


int main()
{
	init();

	std::cout << "Unit service exit. Good-bye)" << std::endl;
	return 0;
}
