#ifndef AccessDeviceUnit_Included
#define AccessDeviceUnit_Included

#include <datatypes/location.pb.h>
#include <contracts/locations/ilocation.hpp>
#include <contracts/observers/observable..hpp>
#include <contracts/devices/access_device/icommand_result.hpp>

namespace tracking
{
	namespace units
	{
		typedef contracts::devices::access_device::ICommandResult ICommandResult;

		class IDeviceUpdatable
		{
		public :
			virtual ~IDeviceUpdatable()	{}

			virtual void update(const std::string& device_name) = 0;
		};

		template <typename T>
		class IIdentification
		{
		public:
			virtual ~IIdentification() {}

			virtual DataTypes::VisitRecord verify  (const DataTypes::VisitRecord& target, const T& data) = 0;
			virtual DataTypes::VisitRecord identify(const T& data) = 0;
		};

		class IAccessCoordinator
		{
		public:
			virtual ~IAccessCoordinator() {}

			virtual void grant() = 0;
			virtual void deny () = 0;
		};




		class AccessDeviceObserver : public contracts::observers::Observable
	                             , contracts::devices::IDeviceObserver<ICommandResult>
			                         , IDeviceUpdatable
			                         , contracts::common::ILifecycle
			                         , IIdentification<std::string>
			                         , IAccessCoordinator
		{

		public:
			virtual ~AccessDeviceObserver() {}

			explicit AccessDeviceObserver()
			{
				
			}

			void update(const std::string& device_name) override
			{

			}

			void grant() override
			{

			}

			void deny() override
			{

			}

			DataTypes::VisitRecord verify( const DataTypes::VisitRecord& target
				                           , const std::string& data) override
			{				
				return DataTypes::VisitRecord();
			}
			
			DataTypes::VisitRecord identify(const std::string& data) override
			{
				return DataTypes::VisitRecord();
			}
			
			void on_error(const contracts::devices::DeviceException& exception) const override
			{
				
			}

			void on_state(const contracts::devices::IDeviceState& state) const override
			{
				
			}

			void on_next(const ICommandResult& data) const override
			{
				
			}

			

		};
	}
}

#endif
