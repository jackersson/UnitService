#ifndef TestableTrackLocation_Included
#define TestableTrackLocation_Included

#include <contracts/locations/ilocation.hpp>
#include "units/access_device_unit.hpp"

#include <data/models/location.hpp>

namespace track_locations_tests
{	
		class TestableTrackLocation : public contracts::locations::ILocation
		{
		public:
			TestableTrackLocation()	{}

			virtual ~TestableTrackLocation() {}
		
			void update(const data_model::Location& val) override
			{
				location_ = val;			
			}

			void start() override	{}

			void stop() override	{}

			void on_target_detected() override {
				throw std::exception("Not implemented exception");
			}

			void on_target_detected(data_model::VisitRecord& object) override {}

			const data_model::Location& location() const override {
				return location_;
			}

			const contracts::devices::access_device::IAccessCoordinator&
				access_coordinator() const override {
				throw std::exception();
			}

			void on_error(const contracts::devices::DeviceException& exception)  override
			{}

			void on_state(const contracts::devices::IDeviceState& state) override
			{}

		private:	
			TestableTrackLocation(const TestableTrackLocation& other) = delete;
			TestableTrackLocation& operator=(const TestableTrackLocation&) = delete;
					
			data_model::Location        location_;
			contracts::logging::Logger  logger_  ;
		};

		typedef std::shared_ptr<TestableTrackLocation> TestableTrackLocationPtr;
	
}

#endif
