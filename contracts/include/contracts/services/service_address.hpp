#ifndef IServiceAddress_INCLUDED
#define IServiceAddress_INCLUDED

#include <string>
#include <sstream>

namespace contracts
{
	namespace services
	{
		class IServiceAddress
		{
		public:
			virtual ~IServiceAddress() {}

			virtual const std::string&  host() const = 0;
			virtual unsigned int        port() const = 0;
			virtual const std::string&  get() const = 0;
		};

		class ServiceAddress : public IServiceAddress
		{
		public:
			explicit ServiceAddress(const std::string& ip_address)
				: ip_address_("")
				, port_(0)
				, formatted_address_(ip_address)
			{}

			ServiceAddress( const std::string& ip_address 
			              , unsigned int port)
				            : ip_address_(ip_address)
				            , port_(port)
			{
				std::stringstream stream;
				stream << ip_address << ":" << port;
				formatted_address_ = stream.str();
			}

			~ServiceAddress() {}

			const std::string&  host() const override	{
				return ip_address_;
			}

			unsigned int  port() const override
			{
				return  port_;
			}

			const std::string& get() const override { return formatted_address_; }

		private:
			std::string  ip_address_;
			unsigned int port_;
			std::string  formatted_address_;
		
		};
	}	
}

#endif