#ifndef AccessDeviceUnit_Included
#define AccessDeviceUnit_Included

#include <contracts/locations/ilocation.hpp>
#include <observers/observable.hpp>
#include <contracts/devices/access_device/icommand_result.hpp>
#include <contracts/devices/access_device/iaccess_coordinator.hpp>
#include <contracts/devices/access_device/iaccess_device_engine.hpp>
#include <future>
#include "idevice_unit.hpp"
#include <common/logger.hpp>
#include <data/irepository.hpp>

#include <data/models/visit_record.hpp>
#include <data/models/devices.hpp>
namespace tracking
{
	namespace units
	{	
		struct AccessDeviceUnitContext
		{
			contracts::devices::access_device::IAccessDeviceEngine* engine    ;
			contracts::data::AbstractRepositoryContainer*           repository;
		};
		
		class AccessDeviceObserver :
			  public contracts::observers::Observable<contracts::locations::ILocation>
	    , public contracts::devices::IDeviceObserver<ICommandResult>
			, public IIdentification<std::string>
			, public contracts::devices::access_device::IAccessCoordinator
		{

		public:
			virtual ~AccessDeviceObserver()	{
				AccessDeviceObserver::stop();
			}

			explicit AccessDeviceObserver	(const AccessDeviceUnitContext& context)			
			{	
				//TODO handle nulls 
				engine_             = context.engine;
				persons_repository_ = context.repository->get<data_model::Person>();
			}		

			void update(const data_model::AccessDevice& device) override
			{
				if (device_ == device)
					return;
				stop();
				device_ = device;
				start();
			}

			void set_state(data_model::AccessState  state) const override
			{
				switch (state)
				{
				case data_model::Granted:
					grant();
					break;
				case data_model::Denied:
					deny();
					break;

				case data_model::NoneState:
				default: break;
				}
			}

			void start() override
			{
				auto dev_name = device_.name;
				if (dev_name == "")
				{
					logger_.error("Device name is not valid");
					return;
				}

				engine_->add(dev_name);
				engine_->subscribe(this, dev_name);
			}

			void stop() override
			{
				engine_->unsubscribe(this);
				engine_->remove(device_.name);
			}

			void grant() const
			{				
				engine_->execute( device_.name
					              , contracts::devices::access_device::lGreenAccess);

				std::future<void> result(std::async([this]()
				  { std::this_thread::sleep_for(ACCESS_DELAY);
				    deny(); }
				));
			}

			void deny() const	{
				engine_->execute( device_.name
					              , contracts::devices::access_device::lRedMain);
			}

			bool verify( data_model::VisitRecord& target
				         , const std::string& data) override
			{				
				data_model::Card card;
				auto person_found = try_extract_card(data, card);
				target.set_card(card);				
				return person_found;
			}
			
			std::shared_ptr<data_model::VisitRecord>
				identify(const std::string& data) override
			{
				data_model::Card card;
				auto person_found = try_extract_card(data, card);

				auto visit_record = std::make_shared<data_model::VisitRecord >();
				visit_record->set_now();
				visit_record->set_card(card);

				if (person_found)
					visit_record->set_person_id(card.owner_id());

				return visit_record;
			}							
			
			void on_error(const contracts::devices::DeviceException& exception) override
			{			
				logger_.error("Access device exception {0}", exception.what());
				for (auto observer : observers_ )
					observer->on_error(exception);
			}

			void on_state(const contracts::devices::IDeviceState& state) override
			{
				//context_->logger()->error("Access device state changed {0}", state.state());
				for (auto observer : observers_)
					observer->on_state(state);
			}

			void on_next(const ICommandResult& data) override
			{
				//context_->logger()->error( "Access device state changed {0}"
					                  //     , data.module());
				switch (data.module())
				{
				case contracts::devices::access_device::Buttons: 
					check_buttons(data.data());
					break;
				case contracts::devices::access_device::Dallas:
					check_dallas_key(data.to_string());
					break;
				case contracts::devices::access_device::Lights: 
				case contracts::devices::access_device::NoneModule:
				default: break;
				}
			}

		private:

			AccessDeviceObserver(const AccessDeviceObserver& other) = delete;
			AccessDeviceObserver& operator=(const AccessDeviceObserver&) = delete;

			bool device_connected() const	{
				return engine_->device_enumerator().connected(device_.name);
			}

			void check_buttons(const std::vector<unsigned char>& data) const
			{
				//TODO flag check utils
			}

			void check_dallas_key(std::string data) {
				if (data == "") //TODO make is_empty function
					return;

				try
				{
					auto target = identify(data);
					on_target_detected(*target);
				}
				catch (std::exception&)	{
					//not implemented
				}
			}

			void on_target_detected (data_model::VisitRecord& visit_record)
			{
				for (auto observer : observers_)
					observer->on_target_detected(visit_record);
			}

			bool try_extract_card(const std::string& data, data_model::Card& card) const
			{
				card.set_id(data_model::Key(data));
				data_model::GetPersonRequest person_request;
				person_request.set_card(card);

				data_model::GetRequest request(person_request);

				std::vector<data_model::Person> items;
				persons_repository_->get(data_model::GetRequest(person_request), items);

				auto person_find = items.size() > 0;
				if (!person_find)
					return false;
				auto person = items[0];
				card.set_owner_id(person.id());

				return true;
			}

			data_model::AccessDevice device_;
			contracts::devices::access_device::IAccessDeviceEngine* engine_;
			contracts::data::IRepository<data_model::Person>* persons_repository_;
			contracts::logging::Logger logger_;
		
			const std::chrono::seconds ACCESS_DELAY = std::chrono::seconds(3);
		};
	}
}

#endif
