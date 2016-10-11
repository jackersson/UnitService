#include <iostream>
#include <numeric>
#include <vector>
#include <future>
/*
#include <serial/serial.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <thread>
#include "access_device/serial_port_enumerator.hpp"
#include "access_device/access_device_engine.hpp"
#include "include/access_device/core/timeout_serial.hpp"
*/
using namespace std;

//boost::asio::io_service io;
//boost::asio::serial_port sp(io);

int accumulate_block_worker_ret(int* data, size_t count) {
	return std::accumulate(data, data + count, 0);
}

int use_worker_in_std_async() {
	vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8 };
	auto fut = std::async(
		std::launch::async, accumulate_block_worker_ret, v.data(), v.size());
	return fut.get();
}



void set(int i, promise<int>& promis)
{
	promis.set_value(i);
}

int get()
{
	auto pr = promise<int>();
	set(5, pr);

	return	pr.get_future().get();		
}

int main()
{
//	auto fut = use_worker_in_std_async();
	std::cout << "use_worker_in_std_async computed " << get() << "\n";
	//get();
	cin.get();
	return 0;
}