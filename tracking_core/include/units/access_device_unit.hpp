#ifndef AccessDeviceUnit_Included
#define AccessDeviceUnit_Included

#include <datatypes/location.pb.h>
#include <contracts/locations/ilocation.hpp>
#include <contracts/observers/observable..hpp>
#include <contracts/devices/access_device/icommand_result.hpp>
#include <contracts/devices/access_device/iaccess_coordinator.hpp>
#include <contracts/devices/access_device/iaccess_device_engine.hpp>
#include <contracts/iunit_context.hpp>

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
			, public IDeviceUpdatable
			, public contracts::common::ILifecycle
			, public IIdentification<std::string>
			, public contracts::devices::access_device::IAccessCoordinator
			, public std::enable_shared_from_this<contracts::devices::IDeviceObserver<ICommandResult>>
		{

		public:
			virtual ~AccessDeviceObserver() {}

			explicit AccessDeviceObserver	(contracts::IUnitContextPtr context)
				: context_(context)
			{
				engine_ = context_->devices()->access_device_engine();
			}

			void update(const std::string& device_name) override
			{
				//TODO check on empty
				stop();

				device_name_ = device_name;
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

				engine_->add(device_name_);
				engine_->subscribe(shared_from_this(), device_name_);
			}

			void stop() override
			{
				//engine_->unsubscribe(shared_from_this());
				engine_->remove(device_name_);
			}

			void grant() const
			{
				//TODO add timer to switch light back
				engine_->execute(device_name_, contracts::devices::access_device::lGreenAccess);
			}

			void deny() const
			{
				engine_->execute(device_name_, contracts::devices::access_device::lRedMain);

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
				//TODO fix car find
				DataTypes::Card* card = nullptr; //context_->repository()->cards()->get();
				auto visit_record = new DataTypes::VisitRecord();
				if (try_extract_card(data, card))
				{
					visit_record->set_allocated_card(card);
					auto person = new DataTypes::Key(card->owner_id());
					visit_record->set_allocated_person_id(person);
					visit_record->set_time(clock());
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
				return engine_->device_enumerator().connected(device_name_);
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

			std::string device_name_;

			contracts::devices::access_device::IAccessDeviceEnginePtr engine_;
			contracts::IUnitContextPtr context_;
		
		};
	}
}

#endif
