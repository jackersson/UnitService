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
			const std::vector<DataTypes::Location>& get(void* request) override
			{
				std::vector<DataTypes::Location> data;
				auto target_request = static_cast<DataTypes::GetLocationRequest*>(request);
				if (target_request == nullptr)
					return data;
			 return get(target_request);
			}

			const std::vector<DataTypes::Location>& get(DataTypes::GetLocationRequest* request)
			{
				std::vector<DataTypes::Location> data;
				Services::GetRequest service_request;
				service_request.set_allocated_location_request(request);
				auto result = api_->get(service_request);

				for (auto item : result->items().items())
				{
					if (item.value_type_case() != Services::Entity::ValueTypeCase::kLocation)
					{
						std::cout << "wrong entity format in locations";
						continue;
					}
					data.push_back(item.location());
				}

				return data;
			}

			const DataTypes::Location& find(DataTypes::Key key) override {
				DataTypes::GetLocationRequest request;
				//TODO set key
				auto response = get(&request);
				return response[0]; //TODO check if exists first item
			}

			//TODO implement
			const DataTypes::Location& find(Services::Entity entity) override
			{
				DataTypes::GetLocationRequest request;
				//TODO set key
				auto response = get(&request);
				return response[0]; //TODO check if exists first item
			}

			//TODO pass by reference
			bool add(DataTypes::Location* entity) override
			{
				Services::Entity service_entity;
				service_entity.set_allocated_location(entity);

				Services::CommitRequest request;
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
