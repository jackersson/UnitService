#ifndef TestableUnitContext_Included
#define TestableUnitContext_Included
#include <contracts/iservice_context.hpp>
#include <devices_container.hpp>

namespace testable_unit_context
{	
	class TestableServiceContext : public contracts::common::IModule
	                            	, public contracts::IServiceContext
	{
	public:
		explicit TestableServiceContext
		(contracts::IServiceConfiguration* configuration)
		    : configuration_(configuration){}
		~TestableServiceContext() {}
		
		void init() override {

			devices_ = std::make_unique<DevicesContainer>();
		}
		void de_init() override {
		}

		contracts::devices::IDevicesContainer* devices() override {
			return devices_.get();
		}

		contracts::data::AbstractRepositoryContainer* repository() override {
			return nullptr;
		}

		contracts::services::IServices*  services() override {
			return nullptr;
		}

		contracts::IServiceConfiguration*  configuration() override {
			return configuration_;
		}

		contracts::locations::ITrackLocationsEngine* track_locations() override
		{
			return nullptr;
		}

	private:
		contracts::IServiceConfiguration* configuration_;

		std::unique_ptr<contracts::devices::IDevicesContainer>        devices_;

		contracts::logging::Logger logger_;
	};
}

#endif

