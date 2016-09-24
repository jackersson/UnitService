#ifndef ConcurrentMap_Included
#define ConcurrentMap_Included

#include <map>
#include <mutex>

namespace utils
{
	template <typename Key, typename Value>
	class ConcurrentMap
	{
	public:
		ConcurrentMap()		{}
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
}

#endif
