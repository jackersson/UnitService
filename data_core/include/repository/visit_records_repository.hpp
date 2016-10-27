#ifndef VisitRecordsRepository_Included
#define VisitRecordsRepository_Included

#include <contracts/data/irepository.hpp>
#include <contracts/services/idatabase_api.hpp>

namespace data_core
{
	namespace datacontext
	{
		class VisitRecordsRepository
			: public contracts::data::IRepository<DataTypes::VisitRecord>
		{
		public:
			explicit
				VisitRecordsRepository(IDataContext<DataTypes::VisitRecord>* datacontext)
				: datacontext_(datacontext)
			{}

			bool get(void* request, std::vector<DataTypes::VisitRecord>& entities) override
			{
				return datacontext_->get(request, entities);
			}

			bool find(DataTypes::Key key, DataTypes::VisitRecord& entity) override {
				return datacontext_->find(key, entity);
			}

			bool add(DataTypes::VisitRecord* entity) override
			{
				return datacontext_->add(entity);
			}

			bool remove(DataTypes::VisitRecord* entity) override
			{
				return datacontext_->remove(entity);
			}

			bool update(DataTypes::VisitRecord* entity) override
			{
				return datacontext_->update(entity);
			}

			std::shared_ptr<contracts::data::ILocalStorage<DataTypes::VisitRecord>>
				local() override
			{
				return nullptr;
			}

		private:
			IDataContext<DataTypes::VisitRecord>* datacontext_;
		};


	}
}

#endif
