#ifndef ICommandResult_Included
#define ICommandResult_Included
#include <vector>
#include <memory>
#include "access_device_types.hpp"

namespace contracts
{
	namespace devices
	{
		namespace access_device {

			class ICommandResult
			{
			public:
				virtual ~ICommandResult() {}

				virtual bool is_valid() const = 0;
				virtual bool is_empty() const = 0;

				virtual uint16_t      device_number () const = 0;
				virtual unsigned char small_data    () const = 0;
				virtual std::string   get_dallas_key() const = 0;

				virtual const std::vector<unsigned char>& full_data() const = 0;

				virtual access_device_module device_module() const = 0;			
			};

			typedef std::shared_ptr<ICommandResult> ICommandResultPtr;

		}
	}
}

#endif
