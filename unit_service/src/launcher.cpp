#include "launcher.hpp"
#include <boost/filesystem.hpp>

 const std::string Launcher::config_filename_ = "config.bio";

 void save_default_config(const std::string& filename )
 {
	 UnitServiceConfiguration::default_configuration().save(filename);
 }

 void create_file(const std::string& filename)
 {
	 std::fstream outfile(filename, std::fstream::out);
	 outfile.close();
 }

 void Launcher::init()
 {
	 auto current_path = boost::filesystem::current_path();
	 std::stringstream stream;
	 stream << "\\" << config_filename_;
	 current_path.concat(stream.str());

	 auto string_path = current_path.string();
	 if (!exists(current_path))
	 {
		 create_file(string_path);
		 save_default_config(string_path);

		 std::cout << "New config file created. Setup config in " << current_path << std::endl;
		 return;
	 }

	 auto ok = configuration_->load(string_path);
	 if (!ok)
		 save_default_config(string_path);
	 unit_service_ = std::make_shared<UnitService>(configuration_);
	 unit_service_->init();
 }

 void Launcher::de_init()
 {
	 if ( unit_service_ != nullptr )
	   unit_service_->de_init();
 }

