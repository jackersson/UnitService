#include <iostream>
#include "include/realsense_device_enumerator.hpp"

using namespace std;
using namespace realsense_device;

int main()
{
	realsense_device_enumerator enumerator;
	enumerator.start();

	this_thread::sleep_for(chrono::seconds(10));

	enumerator.stop();

	cout << "works" << endl;

	cin.get();
	return 0;
}