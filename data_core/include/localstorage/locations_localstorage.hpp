#ifndef LocationsLocalStorage_Included
#define LocationsLocalStorage_Included

#include <contracts/data/irepository.hpp>

namespace data_core
{
	namespace localstorage
	{
		class LocationsLocalStorage
			: public contracts::data::ILocalStorage<DataTypes::Location>
		{
		public:
			const std::vector<DataTypes::Location>& entities() override
			{
				return entities_;
			}

			void subscribe(std::function<void()> func) override
			{
				
			}

			void unsubscribe(std::function<void()> func) override
			{
				
			}

			void unsubscribe_all() override
			{
				
			}

		private:
			std::vector<DataTypes::Location> entities_;
		};


	}
}

#endif
