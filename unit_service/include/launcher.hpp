#ifndef Launcher_Included
#define Launcher_Included
#include <string>

#include "unit_service_configuration.hpp"
#include "unit_service.hpp"

class Launcher : contracts::common::IModule
{
public:
	virtual ~Launcher()	{
		Launcher::de_init();
	}

	void init() override;

	void de_init() override;

	const UnitServiceConfiguration& configuration() const	{
		return configuration_;
	}

	UnitService& context() {
		return unit_service_;
	}

	static const std::string CONFIG_NAME; //= "config.bio";

private:
	UnitServiceConfiguration configuration_;
	UnitService unit_service_;

};

#endif