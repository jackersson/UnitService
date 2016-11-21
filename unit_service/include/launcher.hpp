#ifndef Launcher_Included
#define Launcher_Included
#include <string>

#include "service_configuration.hpp"
#include "service_context.hpp"

#include "logging/logger.hpp"

class Launcher : contracts::common::IModule
{
public:
	virtual ~Launcher()	{
		Launcher::de_init();
	}

	void init() override;

	void de_init() override;

	const ServiceConfiguration& configuration() const	{
		return configuration_;
	}

	ServiceContext& context() {
		return *service_context_;
	}

	static const std::string CONFIG_NAME; //= "config.bio";

private:
	ServiceConfiguration configuration_  ;
	std::unique_ptr<ServiceContext> service_context_;
	contracts::logging::Logger  logger_;
};


bool try_load_config    (contracts::common::IFileIO& configuration);


#endif