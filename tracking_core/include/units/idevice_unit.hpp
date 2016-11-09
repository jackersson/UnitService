#ifndef IDeviceUnit_Included
#define IDeviceUnit_Included

#include <contracts/locations/ilocation.hpp>
#include <contracts/devices/access_device/icommand_result.hpp>
#include <contracts/devices/video_device/istream_data.hpp>

namespace tracking
{
	namespace units
	{
		typedef contracts::devices::access_device::ICommandResult ICommandResult;
		typedef contracts::devices::video_device::IStreamData     IStreamData   ;

		class IDeviceUpdatable
		{
		public:
			virtual ~IDeviceUpdatable() {}

			virtual void update(const std::string& device_name) = 0;
		};

		template <typename T>
		class IIdentification
		{
		public:
			virtual ~IIdentification() {}

			virtual bool verify(data_model::VisitRecord& target, const T& data) = 0;

			virtual std::shared_ptr<data_model::VisitRecord>
				      identify(const T& data) = 0;
		};

		typedef
			std::shared_ptr<contracts::devices::IDeviceObserver<ICommandResult>> IDeviceObserverPtr;
	}
}

#endif