#include "units/access_device_unit.hpp"

#include <data/models/visit_record.hpp>
#include <data/models/devices.hpp>
#include <data/models/queries.hpp>

#include <thread_pool.hpp>

using namespace devices;
using namespace access_device;
using namespace contracts::data;
using namespace data_model;

namespace tracking
{
	namespace units
	{		
		AccessDeviceObserver::~AccessDeviceObserver()			
		{
			AccessDeviceObserver::stop();
		}
			
		AccessDeviceObserver::AccessDeviceObserver(IAccessDeviceEngine*	engine)
			: busy_(false), engine_(engine)
			, persons_repository_(nullptr)
			, device_(std::make_unique<AccessDevice>())
			, active_(false)
		{
			if (engine_ == nullptr)
				throw std::exception("Access device engine can't be null");
		}

		AccessDeviceObserver::AccessDeviceObserver(IAccessDeviceEngine*	engine
		  , AbstractRepositoryContainer* repository)
		  : busy_(false), device_(std::make_unique<AccessDevice>())
			, engine_(engine)
			, active_(false)
		{
			if (engine_ == nullptr)
				throw std::exception("Access device engine can't be null");

			if (repository == nullptr)
				throw std::exception("Repository can't be null");
			
			persons_repository_ = repository->get<Person>();
			if (persons_repository_ == nullptr)
				throw std::exception("Person Repository can't be null");
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
				engine_->execute(*device_, lNone);
			default: break;
			}
		}

		void AccessDeviceObserver::start()
		{
			if (device_->is_empty())
			{
				logger_.error("Device name is not valid");
				return;
			}
			
			engine_->add(*device_);
			engine_->subscribe(this, *device_);
		}

		void AccessDeviceObserver::stop()
		{
			try
			{
				engine_->unsubscribe(this);
				engine_->remove(*device_);
				device_ = std::make_unique<AccessDevice>();

			}
			catch (std::exception&)
			{
				//TODO not implemented
			}

		}

		void AccessDeviceObserver::grant() const
		{	
			if (busy_)
				return;

			busy_ = true;
			logger_.info("Grant access {0}", device_->name());
			utils::threading::submit_job_nowait([this]()
			{
			  engine_->execute( *device_, lGreenAccess);			
				std::this_thread::sleep_for(ACCESS_DELAY);
				deny();
				busy_ = false;
			});
		}

		void AccessDeviceObserver::deny() const  
		{
			logger_.info("Deny access {0}", device_->name());
			engine_->execute(*device_, lRedMain);
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
			set_state(NoneState);
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
			for (auto observer : observers_)
				observer->on_state(state);
		}

		void AccessDeviceObserver::on_next(ICommandResultPtr data) 
		{			
			switch (data->device_module())
			{
			case Buttons:
				check_buttons(data->small_data());
				break;
			case Dallas:
				check_dallas_key(data->get_dallas_key());
				break;
			case Lights:
			case NoneModule:
			default: break;
			}
		}		

		void AccessDeviceObserver::check_buttons(unsigned char data) const
		{
			//TODO flag check utils
		}

		void AccessDeviceObserver::check_dallas_key(const std::string& data) {
			logger_.info("Card detected {0}", data);

			if (data == "" || active_)
			{
				logger_.info("Access device unit is busy", data);
				return;
			}

			active_ = true;
			try
			{
				auto target = identify(data);
				on_target_detected(*target);
			}
			catch (std::exception&) {
				//not implemented
			}


			utils::threading::submit_job_nowait([this]()
			{
				std::this_thread::sleep_for(std::chrono::seconds(ACCESS_DELAY));
				active_ = false;
			});
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
