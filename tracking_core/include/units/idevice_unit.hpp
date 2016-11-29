#ifndef IDeviceUnit_Included
#define IDeviceUnit_Included

#include <locations/ilocation.hpp>

namespace data_model
{
	class VisitRecord;
}

namespace tracking
{
	namespace units
	{	
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
	}
}

#endif