#ifndef DataContextContainer_Included
#define DataContextContainer_Included

#include <contracts/data/irepository.hpp>
#include <contracts/iunit_context.hpp>

#include "datacontext/locations_data_context.hpp"
#include "data_api.hpp"
#include "datacontext/visit_records_datacontext.hpp"
#include "datacontext/persons_data_context.hpp"
#include <any.hpp>

namespace data_core
{
	class DataContextContainer : public contracts::data::AbstractDataContextContainer	
	{		
	public:
		explicit DataContextContainer(contracts::IUnitContext* context)
			: context_(context)
		{}

		explicit DataContextContainer(const DataApi& api)
			: context_(nullptr)
			, api_(api)
		{}

		void init() override
		{
			if (api_.empty())
				api_ = get_api();

			locations_     = std::make_unique<datacontext::LocationsDataContext>(api_);
			container_[typeid(DataTypes::Location).hash_code()] = locations_.get();

			visit_records_ = std::make_unique<datacontext::VisitRecordsDataContext>(api_);
			container_[typeid(DataTypes::VisitRecord).hash_code()] = visit_records_.get();

			persons_       = std::make_unique<datacontext::PersonsDataContext>(api_);
			container_[typeid(DataTypes::Person).hash_code()] = persons_.get();
		}

		void de_init() override
		{}		
		
	private:
		DataApi get_api() const
		{
			DataApi d_api;
			if (context_ != nullptr)
			{
				d_api.set_logger(context_->logger());

				auto services = context_->services();
				if (services != nullptr)
				{
					auto clients = services->clients();
					if (clients != nullptr)
					{
						auto database = clients->database();
						if (database != nullptr)
							d_api.set_api(database.get());
						else
							std::cout << "Database is null" << std::endl;
					}
					else
						std::cout << "Clients are null" << std::endl;
				}
				else
					std::cout << "Services is null" << std::endl;
			}
			else
				std::cout << "Context is null" << std::endl;

			return d_api;
		}

		contracts::IUnitContext* context_;
		DataApi api_;
		std::unique_ptr<contracts::data::IDataContext<DataTypes::Location>> locations_;
		std::unique_ptr<contracts::data::IDataContext<DataTypes::VisitRecord>> visit_records_;
		std::unique_ptr<contracts::data::IDataContext<DataTypes::Person>> persons_;
		
		//std::map<size_t, boost::any> container_;

	};
}

#endif
