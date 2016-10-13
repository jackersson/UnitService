#ifndef LocationsDataContext_Included
#define LocationsDataContext_Included

#include <contracts/data/irepository.hpp>
#include <contracts/services/idatabase_api.hpp>
#include <iostream>

namespace data_core
{
	namespace datacontext
	{
		class LocationsDataContext 
			: public contracts::data::IDataContext<DataTypes::Location>
		{
			
		public:
			explicit LocationsDataContext(contracts::services::IDatabaseApiPtr api)
				: api_(api)
			{				
			}

			//TODO make not reference style
			bool get(void* request, std::vector<DataTypes::Location>& entities) override
			{
				auto target_request = static_cast<DataTypes::GetLocationRequest*>(request);
				if (target_request == nullptr)
					return false;
			 return get(target_request, entities);
			}

			bool get( DataTypes::GetLocationRequest* request
				      , std::vector<DataTypes::Location>& entities)
			{
				DataTypes::GetRequest service_request;
				service_request.set_allocated_location_request(request);
				auto result = api_->get(service_request);

				for (auto item : result->items().items())
				{
					if (item.value_type_case() != DataTypes::Entity::ValueTypeCase::kLocation)
					{
						std::cout << "wrong entity format in locations";
						continue;
					}
					entities.push_back(item.location());
				}

				return true;
			}

			bool find(DataTypes::Key key, DataTypes::Location& result) override
		  {
				DataTypes::GetLocationRequest request;
			
				//TODO set key
				std::vector<DataTypes::Location> entities;
				//TODO common
				auto response = get(&request, entities);
				if (entities.size() > 0)
				{
					result = entities[0];
					return true;
				}
				return false;				
			}
		
			bool add(DataTypes::Location* entity) override
			{
				DataTypes::Entity service_entity;
				service_entity.set_allocated_location(entity);

				DataTypes::CommitRequest request;
				auto mutation = request.add_mutations();
				mutation->set_allocated_insert(&service_entity);

				auto result = api_->commit(request);

				//TODO handle commite response
				return false;
			}

			bool remove(DataTypes::Location* entity) override
			{
				return false;
			}

			bool update(DataTypes::Location* entity) override
			{
				return false;
			}

		private:
			contracts::services::IDatabaseApiPtr api_;
		};


	}
}

#endif
