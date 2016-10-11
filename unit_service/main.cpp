#include <iostream>
#include <contracts/common/ilifecycle.hpp>
#include <contracts/locations/itrack_location_coordinator.hpp>
#include <services/database_service.pb.h>
#include <track_location_coordinator.hpp>
#include <boost/signals2/signal.hpp>
#include <list>
#include "../grpc_module/include/clients_manager.hpp"

/*
template <typename TEntity>
class ILocalStorage
{
public:
	virtual ~ILocalStorage(){}

	virtual const std::list<TEntity>& entities() = 0;
	
	void subscribe(std::function<void()> func)
	{
		signal_.connect(func);
	}

	void unsubscribe(std::function<void()> func)
	{
		signal_.disconnect(func);
	}

	void unsubscribe_all()
	{
		signal_.disconnect_all_slots();
	}

	boost::signals2::signal<void()> signal_;
};

template <typename TEntity>
class IRepository
{
public:
	virtual ~IRepository() {}

	virtual const std::list<TEntity>& get(void* request) = 0;

	virtual const TEntity& find(DataTypes::Key key) = 0;

	virtual const TEntity& find(Services::Entity entity) = 0;

	virtual bool add   (const TEntity& entity) = 0;

	virtual bool remove(const TEntity& entity) = 0;

	virtual bool update(const TEntity& entity) = 0;

	ILocalStorage<TEntity> local_;
};

class IRepositoryContainer : public contracts::common::IModule
{
public:
	std::shared_ptr<IRepository<DataTypes::VisitRecord>> visit_records_;
	std::shared_ptr<IRepository<DataTypes::Location>>    locations_;
};
*/
/*
class IDevicesContainer : public contracts::common::IModule
{
public:

	contracts::devices::access_device::IAccessDeviceEnginePtr access_device_engine_;
};
*/
/*
class IServices : public contracts::common::IModule
{
public:	

	virtual contracts::services::IClientsPtr clients() = 0;
};

class IUnitContext 
{
public:
	virtual ~IUnitContext(){}

	virtual std::shared_ptr<IDevicesContainer>    devices()    = 0;
	virtual std::shared_ptr<IRepositoryContainer> repository() = 0;
};

typedef std::shared_ptr<IUnitContext> IUnitContextPtr;

*/


class UnitService : public contracts::common::IModule
{
public:
	UnitService()
	{
		tracking_coordinator_ 
			= std::make_shared<tracking::locations::TrackLocationCoordinator>();

		//repository_->locations_->local_.subscribe(tracking_coordinator_->);

		//repository_ initialize
	}

	void init() override
	{
		services_->init();
		devices_->init();
		repository_->init();
	}

	void de_init () override
	{
		services_->de_init();
		devices_->de_init();
		repository_->de_init();
	}

	contracts::locations::ITrackLocationsCoordinatorPtr tracking_coordinator_;
	std::shared_ptr<IServices>            services_  ;
	std::shared_ptr<IDevicesContainer>    devices_   ;
	std::shared_ptr<IRepositoryContainer> repository_; //database service
};

int main()
{

	return 0;
}