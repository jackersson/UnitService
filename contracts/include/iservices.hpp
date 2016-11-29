#ifndef ISERVICES_HPP
#define ISERVICES_HPP

#include <services/icoordinator_api.hpp>
#include <services/ifacial_service_api.hpp>
#include <services/iservice.hpp>
#include <data/irepository.hpp>

namespace services
{		
	class IServices : public contracts::services::IService
	{
	public:
		virtual contracts::data::AbstractDataContextContainer* database      () = 0;
		virtual contracts::services::ICoordinatorApi*          coordinator   () = 0;
		virtual contracts::services::IFacialServiceApi*        facial_service() = 0;
	};
}


#endif
