#include "launcher.hpp"
#include <boost/filesystem.hpp>
#include <sstream>
#include <common/ifile_io.hpp>
#include <service_utils.hpp>
#include <logging/logger.hpp>
#include <network_utils.hpp>

const std::string Launcher::CONFIG_NAME = "config.bio";


 void Launcher::init()
 {
	 logger_.info("MacAddress : {0}", utils::network::get_mac_address());
	 logger_.info("IpAddress  : {0}", utils::network::get_local_ip()   );

	 auto ok = try_load_config(configuration_);

	 if (!ok)
		 return;

	 service_context_.set_configuration(&configuration_);
	 service_context_.init();
 }

 void Launcher::de_init() {	
	 service_context_.de_init();
 }

 //TODO to common
 bool try_load_config(contracts::common::IFileIO& configuration)
 {
	 std::string path;

	 auto current_path = boost::filesystem::current_path();
	 std::stringstream stream;
	 stream << "\\" << Launcher::CONFIG_NAME;
	 current_path.concat(stream.str());

	 path = current_path.string();
	 if (!exists(current_path))
	 {
		 utils::service::create_file(path);
		 configuration.save(path);

		 contracts::logging::Logger logger_;
		 logger_.info("New config file created. Setup config in {0} "
			 , path);
		 return false;
	 }

	 auto ok = configuration.load(path);
	 if (!ok)
		 configuration.save(path);

	 return true;
 }
