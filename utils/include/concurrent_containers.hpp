#ifndef ConcurrentMap_Included
#define ConcurrentMap_Included

#include <map>
#include <mutex>

namespace concurrent
{
	namespace containers
	{
		template <typename Key, typename Value>
		class ConcurrentMap
		{
		public:
			ConcurrentMap() {}
			typename std::map<Key, Value>::iterator begin()
			{
				return map_.begin();
			}

			typename std::map<Key, Value>::iterator end()
			{
				return map_.end();
			}

			void clear()
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);
				map_.clear();
			}

			size_t size()
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);
				return map_.size();
			}

			void insert(const Key& key, const Value& value)
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);
				auto val = std::pair<Key, Value>(key, value);
				map_.insert(val);
			}

			void remove(const Key& key)
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);
				auto it = do_find(key);
				if (it != map_.end())
					map_.erase(it);
			}

			bool contains(const Key& key)
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);
				auto it = do_find(key);
				return it != map_.end();
			}

			///<summary> Throws exception if key not found </summary>
			const Value& find(const Key& key)
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);
				auto it = do_find(key);
				if (it != map_.end())
					return it->second;

				throw std::exception("Value by key not found");
			}


		private:
			ConcurrentMap(const ConcurrentMap& other) = delete;
			ConcurrentMap& operator=(const ConcurrentMap&) = delete;

			typename std::map<Key, Value>::const_iterator do_find(const Key& key) const
			{
				return map_.find(key);
			}


			std::map<Key, Value> map_;
			std::recursive_mutex mutex_;
		};


		template <typename Value>
		class ConcurrentVector
		{
		public:
			ConcurrentVector() {}
			typename std::vector<Value>::iterator begin()		{
				return container_.begin();
			}

			typename std::vector<Value>::iterator end()		{
				return container_.end();
			}

			typename std::vector<Value>::const_iterator cbegin() {
				return container_.cbegin();
			}

			typename std::vector<Value>::const_iterator cend() {
				return container_.cend();
			}

			void clear()
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);
				container_.clear();
			}

			size_t size() const
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);
				return container_.size();
			}

			void push_back(const Value& value)
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);
				container_.push_back(value);
			}

			void remove(const Value& value)
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);
				auto it = std::find(container_.begin(), container_.end(), value);
				if ( it != container_.end())				
					container_.erase(it);				
			}

			bool contains(const Value& value)
			{
				std::lock_guard<std::recursive_mutex> lock(mutex_);
				return std::find(container_.begin(), container_.end(), value) != container_.end();
			}

			///<summary> Throws exception if key not found </summary>
			const Value& find(const Value& object)
			{
				auto it = std::find(container_.begin(), container_.end(), object);
				if (it == container_.end())
					throw std::exception("Value by key not found");
				return *it;
			}

		private:
			ConcurrentVector(const ConcurrentVector& other) = delete;
			ConcurrentVector& operator=(const ConcurrentVector&) = delete;


			std::vector<Value> container_;
			mutable std::recursive_mutex mutex_;
		};
	}
}

#endif
