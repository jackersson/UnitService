#ifndef IAccessCoordinator_Included
#define IAccessCoordinator_Included

#include <datatypes/visit_record.pb.h>

namespace contracts
{
	namespace devices
	{
	

		namespace access_device
		{
			class IAccessDeviceUpdatable
			{
			public:
				virtual ~IAccessDeviceUpdatable() {}

				virtual void update(const DataTypes::AccessDevice& device) = 0;
			};

			class IAccessCoordinator: public IAccessDeviceUpdatable
				                      , public common::ILifecycle
			{
			public:
				virtual ~IAccessCoordinator() {}
		
				virtual void set_state(DataTypes::AccessState  state) const = 0;
			};

		}
	}
}

#endif
