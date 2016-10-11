#ifndef SerialPortIO_Included
#define SerialPortIO_Included

#include <chrono>
#include <thread>
#include "timeout_serial.hpp"


namespace access_device
{
	class SerialPortIO
	{
	public:
		virtual ~SerialPortIO() {}

		bool execute( TimeoutSerial& sp
			          , const std::vector<unsigned char>& input
			          , std::vector<unsigned char>& output
			          , size_t total_count
			          , unsigned char header) const
		{
			if (!sp.is_open())
				return false;

			if (!write(sp, input))
				return false;
			
			std::this_thread::sleep_for(delay_);

			read(sp, output, total_count, header);
			return output.size() == total_count;
		}

		bool write( TimeoutSerial& sp
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

		void read( TimeoutSerial& sp
			       , std::vector<unsigned char>& output
			       , size_t total_count
			       , unsigned char header) const
		{
			auto          timeout = 0;
			unsigned char value   = 0;

			output.clear();
			output.push_back(header);
			while (header != value || timeout < 10)
			{
				char readc = 0;
				try
				{
					sp.read(&readc, 1);
				}
				catch (std::exception& ex) {
					std::cout << ex.what() << std::endl;
					break;
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

	private:
		std::chrono::milliseconds delay_ = std::chrono::milliseconds(100);
	};		
}

#endif
