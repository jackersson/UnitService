#include <iostream>

#include <serial/serial.h>
#include <boost/asio.hpp>
#include "include/serial_port_enumerator.hpp"
#include "include/access_device_engine.hpp"
//#include "concurrent_unordered_map.h"

using namespace std;

boost::asio::io_service io;
boost::asio::serial_port sp(io);

int main()
{
	//serial_port::SerialPortEnumerator enumerator;
	//enumerator.start();

	access_device::AccessDeviceEngine engine;


	this_thread::sleep_for(chrono::seconds(3));	
	cout << engine.is_active("COM3") << endl;
	cout << engine.device_enumerator().connected("COM3") << endl;
	engine.stop_all();

	//enumerator.stop();
	/*
	vector<serial::PortInfo> ports = serial::list_ports();
	serial_port.open("COM3");
	serial_port.set_option(boost::asio::serial_port_base::baud_rate(4800));

	cout << serial_port.is_open() << endl;

	

	unsigned char bytes[7] = {130, 0, 0, 8, 0, 8, 10};
	boost::asio::write(serial_port, boost::asio::buffer(bytes, 7));

	serial_port.close();
	*/
	cout << "works" << endl;
	cin.get();
	return 0;


}