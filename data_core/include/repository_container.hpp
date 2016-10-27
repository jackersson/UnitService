#ifndef RepositoryContainer_Included
#define RepositoryContainer_Included

#include <contracts/data/irepository.hpp>
#include <contracts/iunit_context.hpp>
#include "datacontext_container.hpp"
#include "repository/location_repository.hpp"
#include "repository/visit_records_repository.hpp"
#include "repository/persons_repository.hpp"

namespace data_core
{		
	class RepositoryContainer	: public contracts::data::AbstractRepositoryContainer
	{
		typedef contracts::data::IRepository<DataTypes::VisitRecord> IVisitRecordRepository;
		typedef contracts::data::IRepository<DataTypes::Location>	ILocationRepository;
		typedef contracts::data::IRepository<DataTypes::Person>	  IPersonRepository;

	public:
		RepositoryContainer()
			: context_(nullptr)
			, data_context_container_(nullptr)
		{}

		explicit RepositoryContainer(contracts::IUnitContext* context )
			: context_(context)
		{
			datacontext_container_impl_ = std::make_unique<DataContextContainer>(context);
			data_context_container_ = datacontext_container_impl_.get();
		}

		explicit RepositoryContainer(
			        contracts::data::AbstractDataContextContainer* datacontext)
			: context_(nullptr)
			, data_context_container_(datacontext)
		{	}

		void init() override
		{		
			if (data_context_container_ == nullptr)
				return;

			auto loc_dcontext = data_context_container_->get<DataTypes::Location>();
			locations_ 
				= std::make_unique<datacontext::LocationsRepository>(loc_dcontext);
			container_[typeid(DataTypes::Location).hash_code()] = locations_.get();

			auto vis_dcontext = data_context_container_->get<DataTypes::VisitRecord>();
			visit_records_ 
				= std::make_unique<datacontext::VisitRecordsRepository>(vis_dcontext);
			container_[typeid(DataTypes::VisitRecord).hash_code()] = visit_records_.get();

			auto per_dcontext = data_context_container_->get<DataTypes::Person>();
			persons_ = std::make_unique<datacontext::PersonsRepository>(per_dcontext);
			container_[typeid(DataTypes::Person).hash_code()] = persons_.get();
		}

		void de_init() override	{}	

	private:
		contracts::IUnitContext* context_;

		std::unique_ptr<ILocationRepository>	   locations_    ;
		std::unique_ptr<IVisitRecordRepository> visit_records_;
		std::unique_ptr<IPersonRepository>		   persons_      ;

		contracts::data::AbstractDataContextContainer* data_context_container_;

		std::unique_ptr<DataContextContainer> datacontext_container_impl_;
	};	
}

#endif
