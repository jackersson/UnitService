#ifndef ILocation_Included
#define ILocation_Included

#include <contracts/devices/device_observer.hpp>
#include <common/ilifecycle.hpp>
#include <contracts/devices/access_device/iaccess_coordinator.hpp>
#include <data/models/location.hpp>
#include <data/models/visit_record.hpp>
#include <memory>

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
			              , public ITargetDetectionObserver<data_model::VisitRecord>
			              , public common::ILifecycle
		{
		public:
			virtual ~ILocation() {}

			virtual const data_model::Location& location() const = 0;

			virtual void update(const data_model::Location& locatiion) = 0;

			virtual const
				devices::access_device::IAccessCoordinator& 
				  access_coordinator() const = 0;
		};

		typedef std::shared_ptr<ILocation> ILocationPtr;
	}
}

#endif
