#ifndef TestableUnitContext_Included
#define TestableUnitContext_Included
#include <contracts/iunit_context.hpp>
#include <track_locations_engine.hpp>
#include <devices_container.hpp>

namespace testable_unit_context
{
	class TestableUnitServiceConfiguration : public contracts::IUnitConfiguration
	{
	public:
		TestableUnitServiceConfiguration()
			: facial_service_address_("")
			, coordinator_service_address_("")
			, unit_service_port_(0)
		{}

		TestableUnitServiceConfiguration(const TestableUnitServiceConfiguration &obj)
			: facial_service_address_(obj.facial_service_address_)
			, coordinator_service_address_(obj.coordinator_service_address_)
			, unit_service_port_(obj.unit_service_port_)
		{

		}

		const std::string& facial_service_address() const override {
			return facial_service_address_;
		}

		const std::string& coordinator_service_address() const override {
			coordinator_service_address_ = "127.0.0.1:65400";
			return coordinator_service_address_;
		}

		uint16_t    unit_service_port() const override {
			return unit_service_port_;
		}

		const std::string& service_uuid() const override
		{
			service_uuid_ = "2efb5dd1-5970-49b0-9f46-ec866e9282c1";
			return service_uuid_;
		}


		const std::string& database_service_address() const override {
			return "";
		}

		void set_facial_service_address(const std::string& value) const
		{
			facial_service_address_ = value;
		}

		void set_coordinator_service_address(const std::string& value) const
		{
			coordinator_service_address_ = value;
		}

		void    set_unit_service_port(uint16_t value) {
			unit_service_port_ = value;
		}

		bool empty() const
		{
			return    facial_service_address_ == ""
				&& coordinator_service_address_ == ""
				&& unit_service_port_ == 0;
		}

		static TestableUnitServiceConfiguration default_configuration()
		{
			TestableUnitServiceConfiguration config;
			config.set_facial_service_address("127.0.0.1:50051");
			config.set_coordinator_service_address("127.0.0.1:49065");
			config.set_unit_service_port(50053);
			return config;
		}

	private:
		mutable std::string facial_service_address_;
		mutable std::string coordinator_service_address_;
		uint16_t    unit_service_port_;
		mutable std::string service_uuid_;

	};

	class TestableUnitService : public contracts::common::IModule
		, public contracts::IUnitContext
		//, public std::enable_shared_from_this<contracts::IUnitContext>
	{
	public:
		TestableUnitService() : logger_(std::make_shared<contracts::common::Logger>())
			, configuration_
			(new TestableUnitServiceConfiguration(
				TestableUnitServiceConfiguration::default_configuration()))

		{
			//logger_ = std::make_shared<contracts::common::Logger>();
			//configuration_ = std::make_shared<UnitServiceConfiguration>(UnitServiceConfiguration::default_configuration());
			logger_->info("UnitService created");
			//configuration_ = std::shared_ptr<UnitServiceConfiguration>(
			//	UnitServiceConfiguration::default_configuration());
		}

		~TestableUnitService() {
			TestableUnitService::de_init();
			TestableUnitService::logger()->info("Unit service destroyed");
		}

		//TODO refactor
		void init() override
		{
			logger()->info("Unit service start init");

			//auto this_ptr = shared_from_this();
			//services_ = std::make_shared<grpc_services::ServicesCoordinator>(this);
			//services_->init();

			//services_->clients()->coordinator()->connect();
			//connect();

			devices_ = std::make_shared<DevicesContainer>();
			devices_->init();

			tracking_coordinator_ 
				= std::make_shared<tracking::locations::TrackLocationsEngine>(this);
			tracking_coordinator_->init();

			logger()->info("Unit service init done");
		}

		//TODO refactor
		void de_init() override
		{
			if (services_ != nullptr)
				services_->de_init();

			if (devices_ != nullptr)
				devices_->de_init();

			if (repository_ != nullptr)
				repository_->de_init();

			if (tracking_coordinator_ != nullptr)
				tracking_coordinator_->de_init();
		}

		static void connect()
		{
		}

		std::shared_ptr<contracts::devices::IDevicesContainer> devices() override {
			return devices_;
		}

		std::shared_ptr<contracts::data::IRepositoryContainer> repository() override {
			return repository_;
		}

		std::shared_ptr<contracts::services::IServices>        services() override {
			return services_;
		}

		std::shared_ptr<contracts::common::Logger>             logger() override {
			return logger_;
		}

		contracts::IUnitConfiguration&       configuration() override {
			return *configuration_.get();
		}

		std::shared_ptr<contracts::locations::ITrackLocationsEngine>
			track_locations() override
		{
			return tracking_coordinator_;
		}

		std::shared_ptr<contracts::IUnitConfiguration> configuration_;

		contracts::locations::ITrackLocationsEnginePtr tracking_coordinator_;
		std::shared_ptr<contracts::services::IServices>        services_;
		std::shared_ptr<contracts::common::Logger>             logger_;
		std::shared_ptr<contracts::devices::IDevicesContainer> devices_;
		std::shared_ptr<contracts::data::IRepositoryContainer> repository_; //database service
	};
}

#endif

