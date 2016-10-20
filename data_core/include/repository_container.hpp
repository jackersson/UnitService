#ifndef RepositoryContainer_Included
#define RepositoryContainer_Included

#include <contracts/data/irepository.hpp>
#include <contracts/iunit_context.hpp>

#include "repository/location_repository.hpp"
#include "datacontext/locations_data_context.hpp"
#include "data_api.hpp"
#include "datacontext/visit_records_datacontext.hpp"
#include "repository/visit_records_repository.hpp"
#include "repository/persons_repository.hpp"
#include "datacontext/persons_data_context.hpp"

namespace data_core
{		
	class RepositoryContainer	: public contracts::data::IRepositoryContainer	
	{
	public:
		explicit RepositoryContainer(contracts::IUnitContext* context )
			: context_(context)
		{	}

		void init() override
		{
			DataApi d_api;
			d_api.set_logger(context_->logger());

			//TODO here going to be coordinator
			d_api.set_api(context_->services()->clients()->database()); 

			auto locations_datacontext
				= std::make_shared<datacontext::LocationsDataContext>(d_api);
			location_repository_
				= std::make_shared<datacontext::LocationsRepository>(locations_datacontext);

			auto visit_record_datacontext
				= std::make_shared<datacontext::VisitRecordsDataContext>(d_api);
			visit_record_repository_
				= std::make_shared<datacontext::VisitRecordsRepository>(visit_record_datacontext);

			auto persons_datacontext
				= std::make_shared<datacontext::PersonsDataContext>(d_api);
			persons_repository_
				= std::make_shared<datacontext::PersonsRepository>(persons_datacontext);
		}

		void de_init() override
		{

		}

		contracts::data::VisitRecordRepositoryPtr visit_records() override {
			return visit_record_repository_;
		}

		
		contracts::data::LocationRepositoryPtr locations() override	{
			return location_repository_;
		}

		
		contracts::data::PersonRepositoryPtr persons() override	{
			return persons_repository_;
		}

	private:
		contracts::IUnitContext* context_;

		contracts::data::VisitRecordRepositoryPtr visit_record_repository_;
		contracts::data::LocationRepositoryPtr    location_repository_    ;
		contracts::data::PersonRepositoryPtr      persons_repository_     ;
	};	
}

#endif
