#ifndef SerialPortIO_Included
#define SerialPortIO_Included

#include <chrono>
#include <vector>

namespace access_device
{
	class TimeoutSerial;

	class SerialPortIO
	{
	public:
		virtual ~SerialPortIO();

		bool execute( TimeoutSerial& sp
			          , const std::vector<unsigned char>& input
			          , std::vector<unsigned char>& output
			          , size_t total_count
			          , unsigned char header) const;
		
		bool write( TimeoutSerial& sp
			        , const std::vector<unsigned char>& input) const;
		
		void read( TimeoutSerial& sp
			       , std::vector<unsigned char>& output
			       , size_t total_count
			       , unsigned char header) const;
		
	private:
		static std::chrono::milliseconds delay_;
	};		
}

#endif
