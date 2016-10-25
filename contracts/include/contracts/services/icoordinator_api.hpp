#ifndef ICoordinatorApi_INCLUDED
#define ICoordinatorApi_INCLUDED

#include <datatypes/unit.pb.h>

namespace contracts
{
	namespace services
	{
		class IHeartbeat
		{
		public:
			virtual ~IHeartbeat() {}

			virtual void
				heart_beat(const DataTypes::HeartbeatMessage& request) const = 0;

		};

		class ICoordinatorApi : public IHeartbeat
		{
		public:
			virtual ~ICoordinatorApi() {}

			virtual bool 
				connect_request    (const DataTypes::ConnectMsg& request) = 0;		
														     
		
			virtual void
				update_devices(const DataTypes::DeviceUpdate& request) const = 0;		
		};

		typedef std::shared_ptr<ICoordinatorApi> ICoordinatorApiPtr;
	}
}

#endif
