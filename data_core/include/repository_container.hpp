#ifndef RepositoryContainer_Included
#define RepositoryContainer_Included

#include <contracts/data/irepository.hpp>
#include <contracts/iunit_context.hpp>

#include "repository/location_repository.hpp"
#include "datacontext/locations_data_context.hpp"

namespace data_core
{	
	class RepositoryContainer	: public contracts::data::IRepositoryContainer	
	{
	public:
		explicit RepositoryContainer(contracts::IUnitContextPtr context )
			: context_(context)
		{	}

		void init() override
		{
			auto database_api = context_->services()->clients()->database();
			auto locations_datacontext
				= std::make_shared<datacontext::LocationsDataContext>(database_api);
			location_repository_
				= std::make_shared<datacontext::LocationsRepository>(locations_datacontext);
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

		
		contracts::data::CardRepositoryPtr cards() override	{
			return card_repository_;
		}

	private:
		contracts::IUnitContextPtr context_;

		contracts::data::VisitRecordRepositoryPtr visit_record_repository_;
		contracts::data::LocationRepositoryPtr    location_repository_    ;
		contracts::data::CardRepositoryPtr        card_repository_        ;
	};	
}

#endif
