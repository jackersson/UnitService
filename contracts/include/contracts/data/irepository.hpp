#ifndef IRepository_Included
#define IRepository_Included

#include <datatypes/data.pb.h>
#include <services/database_service.pb.h>
#include <contracts/common/ilifecycle.hpp>
#include <functional>

namespace contracts
{
	namespace data
	{
		template <typename TEntity>
		class ILocalStorage
		{
		public:
			virtual ~ILocalStorage() {}

			virtual const std::vector<TEntity>& entities() = 0;

			virtual void subscribe(std::function<void()> func)  = 0;			

			virtual void unsubscribe(std::function<void()> func) = 0;

			virtual void unsubscribe_all() = 0;
			
		};

		template <typename TEntity>
		class IRepository
		{
		public:
			virtual ~IRepository() {}

			virtual const std::vector<TEntity>& get(void* request) = 0;

			virtual const TEntity& find(DataTypes::Key key) = 0;

			virtual const TEntity& find(Services::Entity entity) = 0;

			virtual bool add(const TEntity& entity) = 0;

			virtual bool remove(const TEntity& entity) = 0;

			virtual bool update(const TEntity& entity) = 0;

			virtual std::shared_ptr<ILocalStorage<TEntity>> local() = 0;
		};

		template <typename TEntity>
		class IDataContext : public IRepository<TEntity>
		{
		public:
			~IDataContext() {}
		};

		class IRepositoryContainer : public common::IModule
		{
		public:
			virtual ~IRepositoryContainer() {}

			virtual 
				std::shared_ptr<IRepository<DataTypes::VisitRecord>> visit_records() = 0;

			virtual 
				std::shared_ptr<IRepository<DataTypes::Location>> locations() = 0;

			virtual
				std::shared_ptr<IRepository<DataTypes::Card>> cards() = 0;

		};
	}
}

#endif
