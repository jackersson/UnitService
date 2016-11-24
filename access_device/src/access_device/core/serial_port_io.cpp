#include <chrono>
#include <thread>
#include <iostream>
#include <access_device/core/serial_port_io.hpp>
#include <access_device/core/timeout_serial.hpp>

using namespace std::chrono;

namespace access_device
{
	milliseconds SerialPortIO::delay_ = milliseconds(200);

	SerialPortIO::~SerialPortIO() {}

	bool SerialPortIO::execute( TimeoutSerial& sp
		                        , const std::vector<unsigned char>& input
		                        , std::vector<unsigned char>& output
		                        , size_t total_count
		                        , unsigned char header) const
	{
		if (!sp.is_open())
		{
			std::cout << "SerialPortIO::execute !sp.is_open()" << std::endl;
			return false;
		}

		if (!write(sp, input))
		{
			std::cout << "SerialPortIO::execute !write(sp, input)" << std::endl;
			return false;
		}

		read(sp, output, total_count, header);
		return output.size() == total_count;
	}

	bool SerialPortIO::write(TimeoutSerial& sp
		, const std::vector<unsigned char>& input) const
	{
		try
		{			
			sp.write((char*)input.data(), input.size());
			return true;
		}
		catch (std::exception& ex) {
			std::cout << ex.what() << std::endl;
			return false;
		}
	}

	void SerialPortIO::read( TimeoutSerial& sp
		                     , std::vector<unsigned char>& output
		                     , size_t total_count
		                     , unsigned char header) const
	{
		auto          timeout = clock();
		unsigned char value = 0;
		
		output.clear();
		output.push_back(header);
		//TODO think about timeout
		while (header != value && (clock() - timeout) < 2 * CLOCKS_PER_SEC)
		{
			char readc = 0;
			try
			{
				sp.read(&readc, 1);
			}
			catch (std::exception& ex) {
				std::cout << "SerialPortIO::read " 
					<< ex.what() << std::endl;
			}

			value = readc;
			if (header == value)
			{
				auto data = sp.read(total_count - 1);
				for (auto ch : data)
					output.push_back(ch);
				break;
			}
			timeout++;
		}
	}
}
