#include <iostream>
#include "../../directshow_device/include/directshow_device_enumerator.hpp"

using namespace std;

int main()
{
	directshow_device::directshow_device_enumerator enumerator;
	enumerator.start();

	this_thread::sleep_for(chrono::seconds(10));

	enumerator.stop();

	cout << "works" << endl;

	cin.get();
	return 0;
}