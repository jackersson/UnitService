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

		access_device_module module()
		{
			
		}

		std::vector<unsigned char>& data()
		{
			
		}

		std::string to_string() override
		{
			
		}

		std::exception exception()
		{
			
		}

		bool ok()
		{
			
		}

		bool empty()
		{
			
		}
	private:
		std::exception       exception_;
		access_device_module module_   ;
		bool ok_  ;
		bool empty_;
	};
}

#endif
