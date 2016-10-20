#ifndef LocationsLocalStorage_Included
#define LocationsLocalStorage_Included

#include <contracts/data/irepository.hpp>
#include <mutex>
#include <contracts/data/data_utils.hpp>

namespace data_core
{
	namespace localstorage
	{
		class LocationsLocalStorage
			: public contracts::data::ILocalStorage<DataTypes::Location>
		{
		public:
			LocationsLocalStorage() {}

			~LocationsLocalStorage()	{
				LocationsLocalStorage::clear();
			}

			const DataTypes::Location& operator[](const size_t& i) const override
			{
				if (i >= size())
					throw std::exception("Index is out of range");
				return *entities_[i];
			}

			void clear() override	{
				entities_.clear();
			}

			size_t size() const override 			{
				return entities_.size();
			}
	
			//TODO consider improvement for add range
			bool add(DataTypes::Location* entity) override
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);

				if (contains(entity->id()))
					update(entity);
				else
				{
					auto clone = std::make_shared<DataTypes::Location>(*entity);
					entities_.push_back(clone);
					notify();
				}

				return true;
			}

			bool remove(const DataTypes::Key& key) override
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);

				auto it = find(key);
				if (it != entities_.end())
				{
					entities_.erase(it);
					notify();
					return true;
				}

				return false;
			}

			bool update(DataTypes::Location* entity) override
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);

				auto it = find(entity->id());
				if (it != entities_.end())
				{
					it->get()->CopyFrom(*entity);
					notify();
					return true;
				}
				return false;
			}

			bool contains(const DataTypes::Key& key)
			{
				auto it = find(key);
				return it != entities_.end();
			}

		private:
			std::vector<std::shared_ptr<DataTypes::Location>>::iterator
		  	find(const DataTypes::Key& key)
			{
				auto predicate = [key](std::shared_ptr<DataTypes::Location> ent) {
					return contracts::data::keys_equal(ent->id(), key);
				};

				auto it = std::find_if(entities_.begin(), entities_.end(), predicate);
				return it;					
			}

			

			void notify()
			{
				for (auto observer : observers_)
					observer->on_data();
			}
			

			LocationsLocalStorage(const LocationsLocalStorage&) = delete;
			LocationsLocalStorage& operator=(const LocationsLocalStorage&) = delete;

			std::vector<std::shared_ptr<DataTypes::Location>> entities_;

			std::recursive_mutex mutex_;

		};


	}
}

#endif
