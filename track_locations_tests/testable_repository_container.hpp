#ifndef TestableRepositoryContainer_Included
#define TestableRepositoryContainer_Included

#include <data/irepository.hpp>
#include "repository/face_charst_repository.hpp"
#include <memory>

namespace track_locations_tests
{
	class TestableRepositoryContainer 
		: public contracts::data::AbstractRepositoryContainer
	{
		typedef contracts::data::IRepository<data_model::VisitRecord> IVisitRecordRepository;
		typedef contracts::data::IRepository<data_model::Location>  	ILocationRepository;
		typedef contracts::data::IRepository<data_model::Person>	    IPersonRepository;
		typedef
			contracts::data::IRepository<data_model::FaceTemplate>  IFaceCharstRepository;

	public:
		TestableRepositoryContainer(): data_context_(nullptr)
		{}

		void init() override
		{
			
			//locations_
			//	= std::make_unique<data_core::datacontext::LocationsRepository>(nullptr);
			container_[typeid(data_model::Location).hash_code()] = nullptr;//locations_.get();

			//visit_records_
			//	= std::make_unique<data_core::datacontext::VisitRecordsRepository>(nullptr);
			container_[typeid(data_model::VisitRecord).hash_code()] = nullptr;// visit_records_.get();

			//persons_ = std::make_unique<data_core::datacontext::PersonsRepository>(nullptr);
			container_[typeid(data_model::Person).hash_code()] = nullptr; //persons_.get();

			//face_charst_ = std::make_unique<data_core::datacontext::FaceCharstRepository>(nullptr);
			container_[typeid(data_model::FaceTemplate).hash_code()] = nullptr; //persons_.get();
			//
		}

		void de_init() override {}

	private:
		std::unique_ptr<ILocationRepository>	   locations_;
		std::unique_ptr<IVisitRecordRepository>  visit_records_;
		std::unique_ptr<IPersonRepository>		   persons_;
		std::unique_ptr<IFaceCharstRepository>	 face_charst_;

		contracts::data::AbstractDataContextContainer* data_context_;
	};
}

#endif
