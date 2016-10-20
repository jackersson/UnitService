#ifndef PersonsDataContext_Included
#define PersonsDataContext_Included

#include <contracts/data/irepository.hpp>
#include <contracts/services/idatabase_api.hpp>
#include <iostream>
#include <data_api.hpp>

namespace data_core
{
	namespace datacontext
	{
		class PersonsDataContext
			: public contracts::data::IDataContext<DataTypes::Person>
		{
		public:
			explicit PersonsDataContext(const DataApi& api)
				: context_(api)
			{}

			bool get(void* request, std::vector<DataTypes::Person>& entities) override
			{
				//TODO stupid method
				auto target_request = static_cast<DataTypes::GetPersonRequest*>(request);
				if (target_request == nullptr)
					return false;
				return get(target_request, entities);
			}

			bool get(DataTypes::GetPersonRequest* request
				, std::vector<DataTypes::Person>& entities)
			{
				if (request == nullptr)
					request = new DataTypes::GetPersonRequest();

				DataTypes::GetRequest service_request;
				service_request.set_allocated_person_request(request);
				try
				{
					auto result = context_.api()->get(service_request);
					parse(result, entities);
					return true;
				}
				catch (std::exception&) {
					//TODO handle exception
					return false;
				}
			}

			void parse(std::shared_ptr<DataTypes::GetResponse> response
				, std::vector<DataTypes::Person>& entities) const
			{
				if (response == nullptr)
					return;
				for (auto item : response->items().items())
				{
					if (item.value_type_case() != DataTypes::Entity::ValueTypeCase::kPerson)
					{
						std::cout << "wrong entity format in visitrecords";
						continue;
					}
					entities.push_back(item.person());
				}
			}

			bool find(DataTypes::Key key, DataTypes::Person& result) override
			{
				throw std::exception("Not implemented method");
			}

			bool add(DataTypes::Person* entity) override
			{
				DataTypes::Entity service_entity;
				service_entity.set_allocated_person(entity);

				DataTypes::CommitRequest request;
				auto mutation = request.add_mutations();
				mutation->set_allocated_insert(&service_entity);

				auto result = context_.api()->commit(request);

				//TODO handle commit response
				return false;
			}

			bool remove(DataTypes::Person* entity) override
			{
				throw std::exception("Not implemented method");
			}

			bool update(DataTypes::Person* entity) override
			{
				throw std::exception("Not implemented method");
			}

		private:
			DataApi context_;
		};


	}
}

#endif
