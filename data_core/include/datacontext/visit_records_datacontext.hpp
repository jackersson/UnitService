#ifndef VisitRecordsDataContext_Included
#define VisitRecordsDataContext_Included

#include <contracts/data/irepository.hpp>
#include <contracts/services/idatabase_api.hpp>
#include <iostream>
#include <data_api.hpp>

namespace data_core
{
	namespace datacontext
	{
		class VisitRecordsDataContext
			: public contracts::data::IDataContext<DataTypes::VisitRecord>
		{
		public:
			explicit VisitRecordsDataContext(const DataApi& api)
				: context_(api)
			{}

			bool get(void* request, std::vector<DataTypes::VisitRecord>& entities) override
			{
				//TODO stupid method
				auto target_request = static_cast<DataTypes::GetVisitRecordRequest*>(request);
				if (target_request == nullptr)
					return false;
				return get(target_request, entities);
			}

			bool get(DataTypes::GetVisitRecordRequest* request
				, std::vector<DataTypes::VisitRecord>& entities)
			{
				if (request == nullptr)
					request = new DataTypes::GetVisitRecordRequest();

				DataTypes::GetRequest service_request;
				service_request.set_allocated_visit_record_request(request);
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
				, std::vector<DataTypes::VisitRecord>& entities) const
			{
				if (response == nullptr)
					return;
				for (auto item : response->items().items())
				{
					if (item.value_type_case() != DataTypes::Entity::ValueTypeCase::kVisitRecord)
					{
						std::cout << "wrong entity format in visitrecords";
						continue;
					}
					entities.push_back(item.visit_record());
				}
			}

			bool find(DataTypes::Key key, DataTypes::VisitRecord& result) override
			{
				throw std::exception("Not implemented method");
			}

			bool add(DataTypes::VisitRecord* entity) override
			{
				DataTypes::Entity service_entity;
				service_entity.set_allocated_visit_record(entity);

				auto request = new DataTypes::CommitRequest();
				auto mutation = request->add_mutations();
				mutation->set_allocated_insert(&service_entity);

				auto result = context_.api()->commit(*request);

				if (result == nullptr)
					return false;

				std::vector<DataTypes::VisitRecord> items;
				return parse(result, items);
			}

			bool parse( std::shared_ptr<DataTypes::CommitResponse> response
			        	, std::vector<DataTypes::VisitRecord>& entities) const
			{
				if (response == nullptr)
					return false;
				auto flag = true;
				for (auto item : response->mutation_results())
				{
					auto error_message = item.error();
					if (error_message != "")
					{
						context_.logger()->error("Mutation error {0}", error_message);
						flag = false;
						continue;
					}
					auto entity = item.entity();
					if (entity.value_type_case() == DataTypes::Entity::ValueTypeCase::kVisitRecord)
					  entities.push_back(entity.visit_record());
				}
				return flag;
			}

			bool remove(DataTypes::VisitRecord* entity) override
			{
				throw std::exception("Not implemented method");
			}

			bool update(DataTypes::VisitRecord* entity) override
			{
				throw std::exception("Not implemented method");
			}

		private:
			DataApi context_;
		};


	}
}

#endif
