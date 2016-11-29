#ifndef TrackLocation_Included
#define TrackLocation_Included

#include <iservice_context.hpp>
#include <locations/ilocation.hpp>
#include "units/access_device_unit.hpp"
#include "units/directshow_device_unit.hpp"


namespace tracking
{	
	class TrackLocation : public ::locations::ILocation
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

		const access_device::IAccessCoordinator&access_coordinator() const override;
		
		void on_error(const devices::DeviceException& exception) override;
		
		void on_state(const devices::IDeviceState& state) override;
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
		contracts::data::IRepository<data_model::Location   >* locations_repository_;
	};

	typedef std::shared_ptr<TrackLocation> TrackLocationPtr;
	
}

#endif
