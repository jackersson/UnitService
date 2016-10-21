#ifndef ILocation_Included
#define ILocation_Included
#include <contracts/devices/device_observer.hpp>
#include <datatypes/visit_record.pb.h>
#include <contracts/common/ilifecycle.hpp>
#include <datatypes/location.pb.h>
#include <contracts/devices/access_device/iaccess_coordinator.hpp>

namespace contracts
{
	namespace locations
	{
		template <typename T>
		class ITargetDetectionObserver
		{
		public:
			virtual ~ITargetDetectionObserver() {}

			virtual void on_target_detected() = 0;
			virtual void on_target_detected(T& object) = 0;
		};

		class ILocation : public devices::IDeviceStateObserver
			              , public ITargetDetectionObserver<DataTypes::VisitRecord>
			              , public common::ILifecycle
		{
		public:
			virtual ~ILocation() {}

			virtual const DataTypes::Location& location() const = 0;

			virtual boost::uuids::uuid id() const = 0;

			virtual void update(const DataTypes::Location& locatiion) = 0;

			virtual const
				contracts::devices::access_device::IAccessCoordinator& 
				  access_coordinator() const = 0;
		};

		typedef std::shared_ptr<ILocation> ILocationPtr;
	}
}

#endif
