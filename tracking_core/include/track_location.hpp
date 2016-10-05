#ifndef TrackLocation_Included
#define TrackLocation_Included
#include <vector>
#include <datatypes/location.pb.h>
#include <contracts/locations/ilocation.hpp>

namespace tracking
{
	namespace locations
	{
		class TrackLocation : contracts::locations::ILocation
		{
		public:
			virtual ~TrackLocation() {}

			explicit TrackLocation(const DataTypes::Location& object)
			{
				TrackLocation::start();
			}

			void update(const DataTypes::Location& object )
			{
				
			}

			void start() override 
			{
				
			}

			void stop() override
			{

			}

			void on_target_detected() override {

			}

			void on_target_detected(const DataTypes::VisitRecord& object) override
			{

			}


			const DataTypes::Location& location() const
			{
				
			}

			void grant() override
			{
				
			}

			void on_error(const contracts::devices::DeviceException& exception) const override
			{
				
			}

			void on_state(const contracts::devices::IDeviceState& state) const override
			{

			}
		
		};
	}
}

#endif
