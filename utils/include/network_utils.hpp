#ifndef _MACADDRESS_UTILITY_H
#define _MACADDRESS_UTILITY_H
#include <string>

namespace utils
{
	namespace network
	{	
		class MACAddressUtility
		{
		public:
			static long get_mac_address(unsigned char * result);

			static long get_mac_address(std::string& result);

			static std::string get_local_ip();

		
		private:
#if defined(WIN32) || defined(UNDER_CE)
			static long get_mac_address_msw(unsigned char * result);
#elif defined(__APPLE__)
			static long GetMACAddressMAC(unsigned char * result);
#elif defined(LINUX) || defined(linux)
			static long GetMACAddressLinux(unsigned char * result);
#endif
		};		
	
		inline std::string get_mac_address()
		{
			std::string mac_address;
			MACAddressUtility::get_mac_address(mac_address);
			return mac_address;
		}		

		inline std::string get_local_ip()
		{
			return MACAddressUtility::get_local_ip();
		}

		

	}
}

#endif