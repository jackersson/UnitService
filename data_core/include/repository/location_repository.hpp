#ifndef LocationsRepository_Included
#define LocationsRepository_Included

#include <contracts/data/irepository.hpp>
#include <contracts/services/idatabase_api.hpp>
#include <localstorage/locations_localstorage.hpp>

namespace data_core
{
	namespace datacontext
	{
		class LocationsRepository
			: public contracts::data::IRepository<DataTypes::Location>
		{
		public:
			explicit 
				LocationsRepository(contracts::data::ILocationDataContextPtr datacontext)
				: datacontext_(datacontext)
				, local_(std::make_shared<localstorage::LocationsLocalStorage>())
			{}

			//TODO make not reference style
			const std::vector<DataTypes::Location>& get(void* request) override
			{
				return datacontext_->get(request);
			}
			
			//TODO make not copy style
			const DataTypes::Location& find(DataTypes::Key key) override {
				return datacontext_->find(key);
			}

			const DataTypes::Location& find(Services::Entity entity) override
			{
				return datacontext_->find(entity);
			}

			bool add(DataTypes::Location* entity) override
			{
				return datacontext_->add(entity);
			}

			bool remove(DataTypes::Location* entity) override
			{
				return datacontext_->remove(entity);
			}

			bool update(DataTypes::Location* entity) override
			{
				return datacontext_->update(entity);
			}

			//TODO implement local storage
			std::shared_ptr<contracts::data::ILocalStorage<DataTypes::Location>> 
				local() override
			{
				return local_;
			}

		private:
			contracts::services::IDatabaseApiPtr api_;
			contracts::data::ILocationDataContextPtr datacontext_;
			std::shared_ptr<contracts::data::ILocalStorage<DataTypes::Location>> local_;
		};


	}
}

#endif
