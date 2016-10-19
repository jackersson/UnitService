#include "launcher.hpp"
#include <boost/filesystem.hpp>

 const std::string Launcher::CONFIG_NAME = "config.bio";

bool try_load_config    (UnitServiceConfiguration& configuration);
void create_file        (const std::string& filename);
void save_default_config(const std::string& filename);

 void Launcher::init()
 {
	 auto ok = try_load_config(configuration_);

	 if (!ok)
		 return;

	 unit_service_.set_configuration(&configuration_);
	 unit_service_.init();
 }

 void Launcher::de_init()
 {
	 //if ( unit_service_ != nullptr )
	 //  unit_service_->de_init();
 }

 //TODO refactor
 bool try_load_config(UnitServiceConfiguration& configuration)
 {
	 std::string path;

	 auto current_path = boost::filesystem::current_path();
	 std::stringstream stream;
	 stream << "\\" << Launcher::CONFIG_NAME;
	 current_path.concat(stream.str());

	 path = current_path.string();
	 if (!exists(current_path))
	 {
		 create_file(path);
		 save_default_config(path);

		 std::cout << "New config file created. Setup config in " << current_path << std::endl;
		 return false;
	 }

	 auto ok = configuration.load(path);
	 if (!ok)
		 save_default_config(path);

	 return true;
 }

 void save_default_config(const std::string& filename)
 {
	 UnitServiceConfiguration::default_configuration().save(filename);
 }

 void create_file(const std::string& filename)
 {
	 std::fstream outfile(filename, std::fstream::out);
	 outfile.close();
 }
