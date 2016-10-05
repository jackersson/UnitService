#include <iostream>
#include "include/directshow_device_enumerator.hpp"
#include <thread>

using namespace std;

int main()
{
	directshow_device::DirectshowDeviceEnumerator enumerator;
	enumerator.start();

	this_thread::sleep_for(chrono::seconds(10));

	enumerator.stop();

	cout << "works" << endl;

	cin.get();
	return 0;
}