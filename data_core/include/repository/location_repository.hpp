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

			bool get(void* request, std::vector<DataTypes::Location>& entities) override
			{
				return datacontext_->get(request, entities);
			}
			
			bool find(DataTypes::Key key, DataTypes::Location& entity) override {
				return datacontext_->find(key, entity);
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
