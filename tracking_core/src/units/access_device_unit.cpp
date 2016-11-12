#include "units/access_device_unit.hpp"

#include <data/models/visit_record.hpp>
#include <data/models/devices.hpp>
#include <data/models/queries.hpp>
#include <future>

using namespace contracts::devices;
using namespace access_device;
using namespace contracts::data;
using namespace data_model;

namespace tracking
{
	namespace units
	{		
		AccessDeviceObserver::~AccessDeviceObserver() {
			AccessDeviceObserver::stop();
		}
			
		AccessDeviceObserver::AccessDeviceObserver(IAccessDeviceEngine*	engine)
			: engine_(engine)
			, persons_repository_(nullptr)
		{
			if (engine_ == nullptr)
				throw std::exception("Access device engine can't be null");
		}

		AccessDeviceObserver::AccessDeviceObserver(IAccessDeviceEngine*	engine
		  , AbstractRepositoryContainer* repository)
		  : engine_(engine)
		{
			if (engine_ == nullptr)
				throw std::exception("Access device engine can't be null");

			if (repository == nullptr)
				persons_repository_ = repository->get<Person>();
		}

		void AccessDeviceObserver::update(const AccessDevice& device) 
		{
			if (*device_ == device)
				return;
			stop();
			*device_ = device;
			start();
		}

		void AccessDeviceObserver::set_state(AccessState  state) const 
		{
			switch (state)
			{
			case Granted:
				grant();
				break;
			case Denied:
				deny();
				break;
			case NoneState:
			default: break;
			}
		}

		void AccessDeviceObserver::start()
		{
			auto dev_name = device_->name();
			if (dev_name == "")
			{
				logger_.error("Device name is not valid");
				return;
			}

			engine_->add(dev_name);
			engine_->subscribe(this, dev_name);
		}

		void AccessDeviceObserver::stop()
		{
			try
			{
				engine_->unsubscribe(this);
				engine_->remove(device_->name());
			}
			catch (std::exception&)
			{
				//TODO not implemented
			}

		}

		void AccessDeviceObserver::grant() const
		{
			engine_->execute(device_->name(), lGreenAccess);

			std::async(std::launch::async, [this]()
			{ 
				std::this_thread::sleep_for(ACCESS_DELAY);
			  deny();
			});
		}

		void AccessDeviceObserver::deny() const  
		{
			engine_->execute(device_->name(), lRedMain);
		}

		bool AccessDeviceObserver::verify( VisitRecord& target
			                               , const std::string& data) 
		{
			Card card;
			auto person_found = try_extract_card(data, card);
			target.set_card(card);
			return person_found;
		}

		std::shared_ptr<VisitRecord>
			AccessDeviceObserver::identify(const std::string& data) 
		{
			Card card;
			auto person_found = try_extract_card(data, card);

			auto visit_record = std::make_shared<VisitRecord>();
			visit_record->set_now();
			visit_record->set_card(card);

			if (person_found)
				visit_record->set_person_id(card.owner_id());

			return visit_record;
		}

		void AccessDeviceObserver::on_error(const DeviceException& exception) 
		{
			logger_.error("Access device exception {0}", exception.what());
			for (auto observer : observers_)
				observer->on_error(exception);
		}

		void AccessDeviceObserver::on_state(const IDeviceState& state)
		{
			//context_->logger()->error("Access device state changed {0}", state.state());
			for (auto observer : observers_)
				observer->on_state(state);
		}

		void AccessDeviceObserver::on_next(const ICommandResult& data) 
		{
			//context_->logger()->error( "Access device state changed {0}"
			//     , data.module());
			switch (data.module())
			{
			case Buttons:
				check_buttons(data.data());
				break;
			case Dallas:
				check_dallas_key(data.to_string());
				break;
			case Lights:
			case NoneModule:
			default: break;
			}
		}

		
		bool AccessDeviceObserver::device_connected()  const {
			return engine_->device_enumerator().connected(device_->name());
		}

		void AccessDeviceObserver::check_buttons(const std::vector<unsigned char>& data) const
		{
			//TODO flag check utils
		}

		void AccessDeviceObserver::check_dallas_key(std::string data) {
			if (data == "") //TODO make is_empty function
				return;

			try
			{
				auto target = identify(data);
				on_target_detected(*target);
			}
			catch (std::exception&) {
				//not implemented
			}
		}

		void AccessDeviceObserver::on_target_detected(VisitRecord& visit_record)
		{
			for (auto observer : observers_)
				observer->on_target_detected(visit_record);
		}

		bool AccessDeviceObserver::try_extract_card( const std::string& data
		                                         	 , Card& card) 
		{
			card.set_id(Key(data));
			GetPersonRequest person_request;
			person_request.set_card(card);

			GetRequest request(person_request);

			std::vector<data_model::Person> items;
			persons_repository_->get(GetRequest(person_request), items);

			auto person_find = items.size() > 0;
			if (!person_find)
				return false;
			auto person = items[0];
			card.set_owner_id(person.id());

			return true;
		}		
	}
}
