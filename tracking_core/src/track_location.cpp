#include "track_location.hpp"

#include <data/models/location.hpp>
#include <data/models/visit_record.hpp>

namespace tracking
{
	namespace locations
	{
		TrackLocation::TrackLocation(contracts::IServiceContext* context)
				: location_(std::make_unique<data_model::Location>())
			  , context_(context)
		{
			try_resolve_dependencies();
		}

		TrackLocation::~TrackLocation() {
				TrackLocation::stop();
			}

		TrackLocation::TrackLocation(const data_model::Location& object
				, contracts::IServiceContext* context)
				: context_(context)
			  , location_(std::make_unique<data_model::Location>())
			{
				if (context_ == nullptr)
					throw std::exception("Context can't be null");

				try_resolve_dependencies();
				TrackLocation::update(object);
			}

			void TrackLocation::update(const data_model::Location& val)
			{
				if (!location_->id().is_empty())
				{
					if (val.id().is_empty() || location_->id() != val.id())
						throw std::exception("Track location id != location id");
				}

				*location_ = val;

				if (access_coordinator_ != nullptr)
					access_coordinator_->update(val.access_device());

				if (directshow_device_unit_ != nullptr)
					directshow_device_unit_->update(val.capture_device());
			}

			void TrackLocation::start()
			{
				//TODO put to list of ILifecycle
				if (directshow_device_unit_ != nullptr)
				  directshow_device_unit_->start();

				if (access_coordinator_ != nullptr)
					access_coordinator_->start();
			}

			void TrackLocation::stop()
			{
				//TODO put to list of ILifecycle
				//if (directshow_device_unit_ != nullptr)
					//directshow_device_unit_->stop();

				if (access_coordinator_ != nullptr)
					access_coordinator_->stop();
			}

			void TrackLocation::on_target_detected() {
				throw std::exception("Not implemented exception");
			}

			void TrackLocation::on_target_detected(data_model::VisitRecord& object) 
			{
				logger_.info("On target detected");
				object.set_location_id(location_->id());
				auto state = object.person_id().is_empty()
					? data_model::AccessState::Denied
					: data_model::AccessState::Granted;
				object.set_access_state(state);

#pragma omp sections
				{						
					if (visit_records_repository_ != nullptr)
					{
						auto result = visit_records_repository_->add(object);
						logger_.info("Visit record detected {0}", result);
					}
#pragma omp section
					{
						access_coordinator_->set_state(state);
					}
				}
			}

			const data_model::Location& TrackLocation::location() const  {
				return *location_;
			}

			const contracts::devices::access_device::IAccessCoordinator&
				TrackLocation::access_coordinator() const  {
				return *access_coordinator_.get();
			}

			void TrackLocation::on_error(const contracts::devices::DeviceException& exception) 
			{
				if (context_ == nullptr)
					throw std::exception("Context can't be null");

				logger_.error("Track location device error {0}", exception.what());
			}

			void TrackLocation::on_state(const contracts::devices::IDeviceState& state)
			{
				//context_->logger()->error("Track location device state {0}", state.state());
			}

		
			void TrackLocation::try_resolve_dependencies()
			{
				auto repository = context_->repository();
				if (repository == nullptr)
					throw std::exception("Repository can't be null");

				auto access_device_engine = context_->devices()->access_device_engine();
				access_coordinator_
					= std::make_unique<units::AccessDeviceObserver>(access_device_engine
						, repository);

				access_coordinator_->subscribe(this);
				
				auto direct_show_engine = context_->devices()->directshow_device_engine();
				directshow_device_unit_
				= std::make_unique<units::DirectShowDeviceUnit>( direct_show_engine
				, repository);
				
				visit_records_repository_ = repository->get<data_model::VisitRecord>();
			}
	}
}

