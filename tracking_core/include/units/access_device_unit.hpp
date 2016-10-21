#ifndef AccessDeviceUnit_Included
#define AccessDeviceUnit_Included

#include <datatypes/location.pb.h>
#include <contracts/locations/ilocation.hpp>
#include <contracts/observers/observable..hpp>
#include <contracts/devices/access_device/icommand_result.hpp>
#include <contracts/devices/access_device/iaccess_coordinator.hpp>
#include <contracts/devices/access_device/iaccess_device_engine.hpp>
#include <contracts/iunit_context.hpp>
#include <contracts/data/data_utils.hpp>

namespace tracking
{
	namespace units
	{
		typedef contracts::devices::access_device::ICommandResult ICommandResult;

		class IDeviceUpdatable
		{
		public :
			virtual ~IDeviceUpdatable()	{}

			virtual void update(const std::string& device_name) = 0;
		};

		class IAccessDeviceUpdatable
		{
		public:
			virtual ~IAccessDeviceUpdatable() {}

			virtual void update(const DataTypes::AccessDevice& device) = 0;
		};

		template <typename T>
		class IIdentification
		{
		public:
			virtual ~IIdentification() {}

			virtual DataTypes::VisitRecord* verify  (DataTypes::VisitRecord& target, const T& data) = 0;
			virtual DataTypes::VisitRecord* identify(const T& data) = 0;
		};		

		typedef 
			std::shared_ptr<contracts::devices::IDeviceObserver<ICommandResult>> IDeviceObserverPtr;

		class AccessDeviceObserver :
			  public contracts::observers::Observable<contracts::locations::ILocation>
	    , public contracts::devices::IDeviceObserver<ICommandResult>
			, public IAccessDeviceUpdatable
			, public contracts::common::ILifecycle
			, public IIdentification<std::string>
			, public contracts::devices::access_device::IAccessCoordinator
		{

		public:
			virtual ~AccessDeviceObserver() {}

			explicit AccessDeviceObserver	(contracts::IUnitContextPtr context)
				: context_(context)
			{
				engine_ = context_->devices()->access_device_engine();
			}

			//TODO to utils
			static bool equals( const DataTypes::AccessDevice& first
			           , const DataTypes::AccessDevice& second)
			{
				return first.name() == second.name();
			}

			void update(const DataTypes::AccessDevice& device) override
			{
				if (equals(device_, device))
					return;
				stop();
				device_ = device;
				start();
			}

			void set_state(DataTypes::AccessState  state) const override
			{
				switch (state)
				{
				case DataTypes::Granted:
					grant();
					break;
				case DataTypes::Denied:
					deny();
					break;

				case DataTypes::NoneState:
				default: break;
				}
			}

			void start() override
			{
				if (!device_connected())
					return;

				auto dev_name = device_.name();
				engine_->add(dev_name);
				engine_->subscribe(this, dev_name);
			}

			void stop() override
			{
				engine_->unsubscribe(this);
				engine_->remove(device_.name());
			}

			void grant() const
			{
				//TODO add timer to switch light back
				engine_->execute(device_.name(), contracts::devices::access_device::lGreenAccess);
			}

			void deny() const
			{
				engine_->execute(device_.name(), contracts::devices::access_device::lRedMain);

			}

			DataTypes::VisitRecord* verify( DataTypes::VisitRecord& target
				                            , const std::string& data) override
			{				
				//TODO fix car find
				DataTypes::Card* card = nullptr; //context_->repository()->cards()->get();
				auto entity_found = false;
				if ( try_extract_card(data, card) )
				{
					target.set_allocated_card(card);
					
					//TODO
					//entity_found = card->owner_id() > 0;
				}

				DataTypes::VisitRecord* visit_record = nullptr;
				if (entity_found)
				{
					auto key = new DataTypes::Key(card->owner_id());
					visit_record = new DataTypes::VisitRecord();
					visit_record->set_allocated_person_id(key);
				}
				return visit_record;
			}
			
			DataTypes::VisitRecord* identify(const std::string& data) override
			{
				auto all_request = new DataTypes::GetPersonRequest();
				all_request->set_card(data);

				std::vector<DataTypes::Person> items;
				context_->repository()->persons()->get(all_request, items);

				auto person_find = items.size() > 0;
				auto person      = items[0];

				//TODO fix car find
				DataTypes::Card* card = nullptr; //context_->repository()->cards()->get();
				auto visit_record = new DataTypes::VisitRecord();
				if (try_extract_card(data, card))
				{
					visit_record->set_allocated_card(card);
					auto person_id = new DataTypes::Key(card->owner_id());
					visit_record->set_allocated_person_id(person_id);
					visit_record->set_allocated_time(contracts::data::get_current_time());
				}

				return visit_record;
			}							
			
			void on_error(const contracts::devices::DeviceException& exception) override
			{			
				for (auto observer : observers_ )
					observer->on_error(exception);
			}

			void on_state(const contracts::devices::IDeviceState& state) override
			{
				for (auto observer : observers_)
					observer->on_state(state);
			}

			void on_next(const ICommandResult& data) override
			{
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
				return engine_->device_enumerator().connected(device_.name());
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

			void on_target_detected (DataTypes::VisitRecord& visit_record)
			{
				for (auto observer : observers_)
					observer->on_target_detected(visit_record);
			}

			bool try_extract_card(const std::string& data, DataTypes::Card* card)
			{
				/*
				card = new DataTypes::Card();
				auto key = new DataTypes::Key();
				identifier.identifier = data;
				card->set_allocated_unique_identifier(key);
				DataTypes::Person entity = _entity_finder.FindPersonByCard(card);

				auto person_key = new DataTypes::Key(entity.id());
				if (entity)
					card->set_allocated_owner_id(person_key);

				return entity == null; */
				return false;
			}

			//std::string device_name_;
			DataTypes::AccessDevice device_;
			contracts::devices::access_device::IAccessDeviceEnginePtr engine_;
			contracts::IUnitContextPtr context_;
		
		};
	}
}

#endif
