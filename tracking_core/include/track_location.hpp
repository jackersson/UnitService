#ifndef TrackLocation_Included
#define TrackLocation_Included

#include <datatypes/location.pb.h>
#include <contracts/locations/ilocation.hpp>
#include <contracts/iunit_context.hpp>

namespace tracking
{
	namespace locations
	{
		class TrackLocation : public contracts::locations::ILocation
		{
		public:
			explicit TrackLocation(contracts::IUnitContextPtr context)
				: context_(context)
				, visit_records_repository_(context_->repository()->visit_records())
			{}

			virtual ~TrackLocation() {}

			explicit TrackLocation(const DataTypes::Location& object
				                   , contracts::IUnitContextPtr context)
				: context_(context)
				, visit_records_repository_(context_->repository()->visit_records())
			{
				TrackLocation::update(object);
			}

			void update(const DataTypes::Location& object ) override
			{
				//access_coordinator_-
				//TODO check if something changed with track location
				TrackLocation::stop();
				location_ = object;
				TrackLocation::start();
			}

			void start() override 
			{
				
			}

			void stop() override
			{

			}

			void on_target_detected() override {
				//Not implemented
			}

			void on_target_detected(DataTypes::VisitRecord& object) override
			{
				auto location_id = new DataTypes::Key(location_.id());
				object.set_allocated_location_id(location_id);
				//TODO check guid on empty
				auto state = object.person_id().guid() != ""
					? DataTypes::AccessState::Granted
					: DataTypes::AccessState::Denied;
				object.set_state(state);

				//TODO make in parallel
				visit_records_repository_->add(&object);
				//parallel
				access_coordinator_->set_state(state);
			}
			
			const DataTypes::Location& location() const override	{
				return location_;
			}

			const contracts::devices::access_device::IAccessCoordinator& 
				access_coordinator() const override {
				return *access_coordinator_.get();
			}
			
			void on_error(const contracts::devices::DeviceException& exception)  override
			{
				
			}

			void on_state(const contracts::devices::IDeviceState& state) override
			{

			}

		private:
			TrackLocation(const TrackLocation& other) = delete;
			TrackLocation& operator=(const TrackLocation&) = delete;

		private:
			std::shared_ptr<contracts::devices::access_device::IAccessCoordinator> access_coordinator_;
			DataTypes::Location location_;
			contracts::IUnitContextPtr context_;
			contracts::data::VisitRecordRepositoryPtr visit_records_repository_;

		
		};

		typedef std::shared_ptr<TrackLocation> TrackLocationPtr;
	}
}

#endif
