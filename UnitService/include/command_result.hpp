#ifndef CommandResult_Included
#define CommandResult_Included
#include <vector>
#include "access_device_types.hpp"
#include "icommand_result.hpp"

namespace access_device
{
	class CommandResult : public ICommandResult
	{
	public:
		virtual ~CommandResult() {}

		access_device_module module() const override
		{
			return module_;
		}

		const std::vector<unsigned char>& data()  const override
		{
			return;
		}

		std::string to_string() const override
		{
			return "";
		}

		const std::exception& exception() const override
		{
			return exception_;
		}

		bool ok() const override
		{
			return ok_;
		}

		bool empty() const override
		{
			return empty_;
		}
	private:
		std::exception       exception_;
		access_device_module module_   ;

		bool ok_  ;
		bool empty_;
	};
}

#endif
