#ifndef ILocation_Included
#define ILocation_Included
#include <contracts/devices/device_observer.hpp>
#include <datatypes/visit_record.pb.h>
#include <contracts/common/ilifecycle.hpp>
#include <datatypes/location.pb.h>

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
			virtual void on_target_detected(const T& object) = 0;
		};

		class ILocation : public devices::IDeviceStateObserver
			              , ITargetDetectionObserver<DataTypes::VisitRecord>
			              , common::ILifecycle
		{
		public:
			virtual ~ILocation() {}

			virtual const DataTypes::Location& location() const = 0;

			virtual void grant() = 0;
		};
	}
}

#endif
