#ifndef IRepository_Included
#define IRepository_Included

#include <datatypes/data.pb.h>
#include <contracts/common/ilifecycle.hpp>
#include <contracts/observers/iobservable.hpp>
#include <contracts/observers/observable..hpp>
#include <any.hpp>

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

			//TODO maybe const
			virtual bool contains(const DataTypes::Key& key) = 0;


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

		/*
		typedef
			std::shared_ptr<IDataContext<DataTypes::Location>>	ILocationDataContextPtr;

		typedef
			std::shared_ptr<IDataContext<DataTypes::VisitRecord>> IVisitRecordsDataContextPtr;

		typedef
			std::shared_ptr<IDataContext<DataTypes::Person>> IPersonsDataContextPtr;
			*/
		template <typename TEntity>
		class IRepository : public IDataContext<TEntity>
		{
		public:
			virtual ~IRepository() {}
			
			virtual std::shared_ptr<ILocalStorage<TEntity>> local() = 0;
		};

		/*
		typedef std::shared_ptr<IRepository<DataTypes::VisitRecord>>
			VisitRecordRepositoryPtr;

		typedef std::shared_ptr<IRepository<DataTypes::Location>>	LocationRepositoryPtr;
		typedef std::shared_ptr<IRepository<DataTypes::Person>>	  PersonRepositoryPtr  ;

		*/
		class AbstractRepositoryContainer : public common::IModule
		{
		public:
			virtual ~AbstractRepositoryContainer() {}

			template <typename T>
			IRepository<T>* get()
			{
				auto key = typeid(T).hash_code();
				if (container_.find(key) == container_.end())
					return nullptr;
				auto target = boost::any_cast<IRepository<T>*>(container_[key]);
				return target;
			}

		protected:
			std::map<size_t, boost::any> container_;

		};

		class AbstractDataContextContainer : public common::IModule
		{
		public:
			virtual ~AbstractDataContextContainer() {}

			template <typename T>	
			IDataContext<T>* get()
			{
				auto key = typeid(T).hash_code();
				if (container_.find(key) == container_.end())
					return nullptr;
				auto target = boost::any_cast<IDataContext<T>*>(container_[key]);
				return target;
			}
		protected:
			std::map<size_t, boost::any> container_;
		};
	}
}

#endif
