#include "unit_service.hpp"
//#include <contracts/common/logger.hpp>

int main()
{
	
	auto service = std::make_shared<UnitService>();
	service->init();

	std::cin.get();
	
	return 0;
}
