#ifndef LocationsDataContext_Included
#define LocationsDataContext_Included

#include <contracts/data/irepository.hpp>
#include <contracts/services/idatabase_api.hpp>
#include <iostream>
#include <data_api.hpp>

namespace data_core
{
	namespace datacontext
	{
		class LocationsDataContext 
			: public contracts::data::IDataContext<DataTypes::Location>
		{
			
		public:
			explicit LocationsDataContext(const DataApi& api)
				: context_(api)
			{}

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
				if (request == nullptr)
					request = new DataTypes::GetLocationRequest();

				DataTypes::GetRequest service_request;				
				service_request.set_allocated_location_request(request);
				try
				{					
					auto result = context_.api()->get(service_request);
					parse(result, entities);
					return true;
				}
				catch (std::exception& )	{
					//TODO handle exception
					return false;
				}				
			}

			void parse(std::shared_ptr<DataTypes::GetResponse> response
		          	, std::vector<DataTypes::Location>& entities) const
			{
				if (response == nullptr)
					return;
				for (auto item : response->items().items())
				{
					if (item.value_type_case() != DataTypes::Entity::ValueTypeCase::kLocation)
					{
						std::cout << "wrong entity format in locations";
						continue;
					}
					entities.push_back(item.location());
				}
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

				auto result = context_.api()->commit(request);

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
			DataApi context_;
			//contracts::services::IDatabaseApiPtr api_;
		};


	}
}

#endif
