#ifndef TrackLocation_Included
#define TrackLocation_Included

#include <contracts/locations/ilocation.hpp>
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
			
			explicit TrackLocation( contracts::IServiceContext* context)
				                    : context_(context)
			{
				try_resolve_dependencies();
			}
		
			virtual ~TrackLocation(){
				TrackLocation::stop();
			}

			explicit TrackLocation( const data_model::Location& object
				                    , contracts::IServiceContext* context)
				                    : context_(context)
			{
				try_resolve_dependencies();
				TrackLocation::update(object);
			}		

			void update(const data_model::Location& val ) override
			{
				location_ = val;
				access_coordinator_    ->update(val.access_device ());
				directshow_device_unit_->update(val.capture_device());
			}

			void start() override 
			{
				//TODO put to list of ILifecycle
				directshow_device_unit_->start();
				access_coordinator_    ->start();
			}

			void stop() override
			{
				//TODO put to list of ILifecycle
				directshow_device_unit_->stop();
				access_coordinator_    ->stop();
			}

			void on_target_detected() override {
				throw std::exception("Not implemented exception");
			}

			void on_target_detected(data_model::VisitRecord& object) override
			{
				object.set_location_id(location_.id());				
				auto state = object.person_id().is_empty() 
					? data_model::AccessState::Granted
					: data_model::AccessState::Denied;
				object.set_access_state(state);

#pragma omp sections
				{
					if (visit_records_repository_ != nullptr)
						visit_records_repository_->add(object);
#pragma omp section
					if (visit_records_repository_ != nullptr)
						access_coordinator_->set_state(state);
				}
			}
			
			const data_model::Location& location() const override	{
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

				logger_.error("Track location device error {0}", exception.what());
			}

			void on_state(const contracts::devices::IDeviceState& state) override
			{
				//context_->logger()->error("Track location device state {0}", state.state());
			}

		private:

			void try_resolve_dependencies()
			{
				if (context_ != nullptr)
				{
					auto repository = context_->repository();

					units::AccessDeviceUnitContext access_device_context_;
					access_device_context_.repository = repository;
					access_device_context_.engine     = context_->devices()->access_device_engine();
					access_coordinator_ 
						= std::make_unique<units::AccessDeviceObserver>(access_device_context_);

					units::DirectShowDeviceUnitContext directshow_device_context_;
					directshow_device_context_.repository = repository;
					directshow_device_context_.engine     = context_->devices()->directshow_device_engine();
					directshow_device_unit_
						= std::make_unique<units::DirectShowDeviceUnit>(directshow_device_context_);
					
					if (repository != nullptr)
						visit_records_repository_ = repository->get<data_model::VisitRecord>();
					else
						logger_.error("Repository is null");
				}
				else
					logger_.error("Context is null");
			}

			TrackLocation(const TrackLocation& other) = delete;
			TrackLocation& operator=(const TrackLocation&) = delete;

			std::unique_ptr<contracts::devices::access_device::IAccessCoordinator> access_coordinator_;
			
			std::unique_ptr<units::DirectShowDeviceUnit> directshow_device_unit_;

			data_model::Location        location_;
			contracts::IServiceContext* context_ ;
			contracts::logging::Logger  logger_  ;

			contracts::data::IRepository<data_model::VisitRecord>* visit_records_repository_;
		};

		typedef std::shared_ptr<TrackLocation> TrackLocationPtr;
	}
}

#endif
