#ifndef DeviceException_Included
#define DeviceException_Included

#include <exception>
#include <data/models/devices.hpp>

namespace contracts
{
	namespace devices
	{
		class DeviceException : public std::exception
		{
		public:
			DeviceException(const std::string& message, data_model::DeviceType deviceType)
				: message_(message), device_type_(deviceType)
			{}

			virtual ~DeviceException() {}
			
      const char* what() const throw() override	{
				return message_.c_str();
			}

			data_model::DeviceType device_type() const {
				return device_type_;
			}
		private:
			std::string message_;
			data_model::DeviceType device_type_;
		};
	}
}

#endif

