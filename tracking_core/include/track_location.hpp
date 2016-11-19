#ifndef TrackLocation_Included
#define TrackLocation_Included

#include <contracts/iservice_context.hpp>
#include "units/access_device_unit.hpp"
#include "units/directshow_device_unit.hpp"


namespace tracking
{
	namespace locations
	{
		class TrackLocation : public contracts::locations::ILocation
		{
		public:			
			explicit TrackLocation(contracts::IServiceContext* context);
		
			virtual ~TrackLocation();

			explicit TrackLocation( const data_model::Location& object
				                    , contracts::IServiceContext* context);

			void update(const data_model::Location& val) override;

			void start() override;
			void stop () override;

			void on_target_detected() override;

			void on_target_detected(data_model::VisitRecord& object) override;
			
			const data_model::Location& location() const override;

			const contracts::devices::access_device::IAccessCoordinator&
				access_coordinator() const override;
			
			void on_error(const contracts::devices::DeviceException& exception) override;
			
			void on_state(const contracts::devices::IDeviceState& state) override;
		private:
			void try_resolve_dependencies();

			TrackLocation(const TrackLocation& other) = delete;
			TrackLocation& operator=(const TrackLocation&) = delete;

			std::unique_ptr<units::AccessDeviceObserver> access_coordinator_;
			
			std::unique_ptr<units::DirectShowDeviceUnit> directshow_device_unit_;

			std::unique_ptr<data_model::Location> location_;
			contracts::IServiceContext* context_ ;
			contracts::logging::Logger  logger_  ;

			contracts::data::IRepository<data_model::VisitRecord>* visit_records_repository_;
		};

		typedef std::shared_ptr<TrackLocation> TrackLocationPtr;
	}
}

#endif
