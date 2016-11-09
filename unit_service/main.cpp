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
	
	std::cin.get();
	return 0;
}
