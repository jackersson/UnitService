#ifndef ICommandResult_Included
#define ICommandResult_Included
#include <vector>

namespace access_device
{
	class ICommandResult
	{
	public:
		virtual ~ICommandResult() {}

		virtual access_device_module module() = 0;
		virtual std::vector<unsigned char>& data() = 0;

		virtual std::string    to_string() = 0;
		virtual std::exception exception() = 0;

		virtual bool is_ok() = 0;
		virtual bool is_empty() = 0;
	};
}

#endif
