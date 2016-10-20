#ifndef PersonsRepository_Included
#define PersonsRepository_Included

#include <contracts/data/irepository.hpp>
#include <contracts/services/idatabase_api.hpp>

namespace data_core
{
	namespace datacontext
	{
		class PersonsRepository
			: public contracts::data::IRepository<DataTypes::Person>
		{
		public:
			explicit
				PersonsRepository(contracts::data::IPersonsDataContextPtr datacontext)
				: datacontext_(datacontext)
			{}

			bool get(void* request, std::vector<DataTypes::Person>& entities) override
			{
				return datacontext_->get(request, entities);
			}

			bool find(DataTypes::Key key, DataTypes::Person& entity) override {
				return datacontext_->find(key, entity);
			}

			bool add(DataTypes::Person* entity) override
			{
				return datacontext_->add(entity);
			}

			bool remove(DataTypes::Person* entity) override
			{
				return datacontext_->remove(entity);
			}

			bool update(DataTypes::Person* entity) override
			{
				return datacontext_->update(entity);
			}

			std::shared_ptr<contracts::data::ILocalStorage<DataTypes::Person>>
				local() override
			{
				return nullptr;
			}

		private:
			contracts::services::IDatabaseApiPtr api_;
			contracts::data::IPersonsDataContextPtr datacontext_;
		};


	}
}

#endif
