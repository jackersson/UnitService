#ifndef IRepository_Included
#define IRepository_Included

#include <datatypes/data.pb.h>
#include <services/database_service.pb.h>
#include <contracts/common/ilifecycle.hpp>
#include <functional>
#include <contracts/observers/iobservable.hpp>
#include <contracts/observers/observable..hpp>

namespace contracts
{
	namespace data
	{
		template <typename TEntity>
		class ILocalStorage : public observers::Observable<observers::IObserver>
		{
		public:
			virtual ~ILocalStorage() {}

			virtual void clear() = 0;

			virtual const TEntity& operator[](const size_t& i) const = 0;

			virtual size_t size() const = 0;
			
			virtual bool add   (TEntity* entity) = 0;

			virtual bool remove(const DataTypes::Key& key) = 0;

			virtual bool update(TEntity* entity) = 0;

			/*
			virtual void subscribe(std::function<void()> func)  = 0;			

			virtual void unsubscribe(std::function<void()> func) = 0;

			virtual void unsubscribe_all() = 0;
			*/
			
		};

		template <typename TEntity>
		class IDataContext 
		{
		public:
			virtual ~IDataContext() {}

			virtual bool get ( void* request, std::vector<TEntity>&) = 0;

			virtual bool find( DataTypes::Key key, TEntity&) = 0;

			virtual bool add   (TEntity* entity) = 0;

			virtual bool remove(TEntity* entity) = 0;

			virtual bool update(TEntity* entity) = 0;
		};


		typedef
			std::shared_ptr<IDataContext<DataTypes::Location>>	ILocationDataContextPtr;

		template <typename TEntity>
		class IRepository : public IDataContext<TEntity>
		{
		public:
			virtual ~IRepository() {}
			
			virtual std::shared_ptr<ILocalStorage<TEntity>> local() = 0;
		};

		typedef std::shared_ptr<IRepository<DataTypes::VisitRecord>>
			VisitRecordRepositoryPtr;

		typedef std::shared_ptr<IRepository<DataTypes::Location>>	LocationRepositoryPtr;
		typedef std::shared_ptr<IRepository<DataTypes::Card>>	CardRepositoryPtr;

		

		class IRepositoryContainer : public common::IModule
		{
		public:
			virtual ~IRepositoryContainer() {}

			virtual VisitRecordRepositoryPtr visit_records() = 0;
			virtual LocationRepositoryPtr    locations()     = 0;
			virtual	CardRepositoryPtr        cards()         = 0;
		};
	}
}

#endif
