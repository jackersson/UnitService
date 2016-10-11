#ifndef DeviceException_Included
#define DeviceException_Included

#include <exception>
#include <datatypes/devices.pb.h>

namespace contracts
{
	namespace devices
	{
		class DeviceException : public std::exception
		{
		public:
			DeviceException(const std::string& message, DataTypes::DeviceType deviceType)
				: message_(message), device_type_(deviceType)
			{}

			virtual ~DeviceException() {}
			
      const char* what() const throw() override	{
				return message_.c_str();
			}

			DataTypes::DeviceType device_type() const {
				return device_type_;
			}
		private:
			std::string message_;
			DataTypes::DeviceType device_type_;
		};
	}
}

#endif

