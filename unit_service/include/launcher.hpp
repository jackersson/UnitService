#ifndef Launcher_Included
#define Launcher_Included
#include <string>

#include "unit_service_configuration.hpp"
#include "unit_service.hpp"

class Launcher : contracts::common::IModule
{
public:
	Launcher()
		: configuration_(std::make_shared<UnitServiceConfiguration>())
	{}

	~Launcher()	{
		Launcher::de_init();
	}

	void init() override;

	void de_init() override;

	std::shared_ptr<UnitServiceConfiguration> configuration() const	{
		return configuration_;
	}

private:
	std::shared_ptr<UnitService>              unit_service_ ;
	std::shared_ptr<UnitServiceConfiguration> configuration_;
	static const std::string config_filename_; //= "config.bio";
};

#endif