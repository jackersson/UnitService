#ifndef AccessDeviceUnit_Included
#define AccessDeviceUnit_Included

#include <locations/ilocation.hpp>
#include <observers/observable.hpp>
#include <devices/access_device/iaccess_coordinator.hpp>
#include <devices/access_device/iaccess_device_engine.hpp>
#include "idevice_unit.hpp"
#include <data/irepository.hpp>
#include <logging/logger.hpp>


namespace data_model
{
	class VisitRecord;
	class Person     ;
	class Location   ;
	class Card       ;
}


namespace tracking
{
	namespace units
	{			
		class AccessDeviceObserver :
			  public contracts::observers::Observable<locations::ILocation>
	    , public devices::IDeviceObserver<access_device::ICommandResultPtr>
			, public IIdentification<std::string>
			, public access_device::IAccessCoordinator
		{

		public:
			virtual ~AccessDeviceObserver();

			explicit
				AccessDeviceObserver
				(access_device::IAccessDeviceEngine*	engine);
				

			AccessDeviceObserver
			( access_device::IAccessDeviceEngine*	engine
			, contracts::data::AbstractRepositoryContainer*           repository);
			

			void update(const data_model::AccessDevice& device) override;
		

			void set_state(data_model::AccessState  state) const override;
			

			void start() override;
		

			void stop() override;
			

			void grant() const;
		

			void deny() const;

			bool verify(data_model::VisitRecord& target
				, const std::string& data) override;
			
			std::shared_ptr<data_model::VisitRecord>
				identify(const std::string& data) override;
			
			void on_error(const devices::DeviceException& exception) override;

			void on_state(const devices::IDeviceState& state) override;

			void on_next(access_device::ICommandResultPtr data) override;

		private:

			mutable bool busy_;
			bool active_;

			AccessDeviceObserver(const AccessDeviceObserver& other) = delete;
			AccessDeviceObserver& operator=(const AccessDeviceObserver&) = delete;

			void check_buttons(unsigned char data) const;
			void check_dallas_key(const std::string& data);

			void on_target_detected(data_model::VisitRecord& visit_record);

			bool try_extract_card(const std::string& data, data_model::Card& card);

			std::unique_ptr<data_model::AccessDevice> device_;
			access_device::IAccessDeviceEngine* engine_;
			contracts::data::IRepository<data_model::Person>* persons_repository_;
			mutable contracts::logging::Logger logger_;
		
			const std::chrono::seconds ACCESS_DELAY = std::chrono::seconds(3);
		};
	}
}

#endif
