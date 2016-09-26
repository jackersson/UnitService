#ifndef SerialPortIO_Included
#define SerialPortIO_Included

#include <asio/serial_port.hpp>
#include <asio/write.hpp>
#include <asio/read.hpp>
#include <chrono>
#include <thread>


namespace access_device
{
	class SerialPortIO
	{
	public:
		virtual ~SerialPortIO() {}

		bool execute( boost::asio::serial_port& sp
			          , const std::vector<unsigned char>& input
			          , std::vector<unsigned char>& output
			          , size_t total_count
			          , unsigned char header) const
		{
			if (!sp.is_open())
				return false;

			write(sp, input);

			std::this_thread::sleep_for(delay_);

			read(sp, output, total_count, header);
			return output.size() == total_count;
		}

		void write( boost::asio::serial_port& sp
			        , const std::vector<unsigned char>& input) const
		{
			boost::asio::write(sp, boost::asio::buffer(input, input.size()));
		}

		void read( boost::asio::serial_port& sp
			       , std::vector<unsigned char>& output
			       , size_t total_count
			       , unsigned char header) const
		{
			auto          timeout = 0;
			unsigned char value   = 0;

			output.clear();

			while (header != value || timeout < delay_.count())
			{
				boost::asio::read(sp, boost::asio::buffer(&value, 1));

				if (header == value)
				{
					output.push_back(value);
					for (auto i = 1; i < total_count - 1; ++i)
					{
						value = 0;
						boost::asio::read(sp, boost::asio::buffer(&value, 1));
						output.push_back(value);
					}
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
