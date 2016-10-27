#ifndef TrackLocation_Included
#define TrackLocation_Included

#include <datatypes/location.pb.h>
#include <contracts/locations/ilocation.hpp>
#include <contracts/iunit_context.hpp>
#include <contracts/data/data_utils.hpp>
#include "units/access_device_unit.hpp"

namespace tracking
{
	namespace locations
	{

		class TrackLocation : public contracts::locations::ILocation
		{
		public:
			
			explicit TrackLocation( contracts::IUnitContext* context)
				                    : context_(context)
			{
				try_resolve_dependencies();
			}
		
			virtual ~TrackLocation(){
				TrackLocation::stop();
			}

			explicit TrackLocation( const DataTypes::Location& object
				                    , contracts::IUnitContext* context)
				                    : context_(context)
			{
				try_resolve_dependencies();
				TrackLocation::update(object);
			}		

			void update(const DataTypes::Location& object ) override
			{
				boost::uuids::uuid uid;
				location_ = object;
				access_coordinator_->update(object.access_device());

				contracts::data::get_guid(location_.id(), uid);
				uuid_ = uid;
			}

			void start() override 
			{
				//TODO put to list of ILifecycle
				access_coordinator_->start();
			}

			void stop() override
			{
				//TODO put to list of ILifecycle
				access_coordinator_->stop();
			}

			void on_target_detected() override {
				throw std::exception("Not implemented exception");
			}

			void on_target_detected(DataTypes::VisitRecord& object) override
			{
				auto location_id = new DataTypes::Key(location_.id());
				object.set_allocated_location_id(location_id);
				auto state = !contracts::data::guid_empty(object.person_id()) 
					? DataTypes::AccessState::Granted
					: DataTypes::AccessState::Denied;
				object.set_state(state);

#pragma omp sections
				{
					if (visit_records_repository_ != nullptr)
						visit_records_repository_->add(&object);
#pragma omp section
					if (visit_records_repository_ != nullptr)
						access_coordinator_->set_state(state);
				}
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
				if (context_ == nullptr)
					throw std::exception("Context can't be null");

				context_->logger()->error("Track location device error {0}", exception.what());
			}

			void on_state(const contracts::devices::IDeviceState& state) override
			{
				//context_->logger()->error("Track location device state {0}", state.state());
			}

			boost::uuids::uuid id() const override{
				return uuid_;
			}

		private:

			void try_resolve_dependencies()
			{
				if (context_ != nullptr)
				{
					access_device_context_.logger         = context_->logger();
					access_device_context_.repository     = context_->repository();
					access_device_context_.access_devices = context_->devices()->access_device_engine();
					access_coordinator_ 
						= std::make_unique<units::AccessDeviceObserver>(access_device_context_);

					auto repository = context_->repository();
					if (repository != nullptr)
						visit_records_repository_ = repository->get<DataTypes::VisitRecord>();
					else
						std::cout << "Repository is null" << std::endl;
				}
				else
					std::cout << "Context is null" << std::endl;
			}

			TrackLocation(const TrackLocation& other) = delete;
			TrackLocation& operator=(const TrackLocation&) = delete;

			std::unique_ptr<contracts::devices::access_device::IAccessCoordinator> access_coordinator_;
			
			boost::uuids::uuid uuid_;
			DataTypes::Location location_;
			contracts::IUnitContext* context_;
			contracts::data::IRepository<DataTypes::VisitRecord>* visit_records_repository_;

			units::AccessDeviceUnitContext access_device_context_;
		};

		typedef std::shared_ptr<TrackLocation> TrackLocationPtr;
	}
}

#endif
