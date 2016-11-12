#ifndef DeviceException_Included
#define DeviceException_Included

#include <exception>
#include <string>

namespace data_model
{
	class Devices   ;	
	enum  DeviceType;
}

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

