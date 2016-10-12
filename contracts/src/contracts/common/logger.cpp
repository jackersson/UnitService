#include "contracts/common/logger.hpp"
#include <filesystem>
//#include <boost/filesystem.hpp>
using namespace spdlog;

namespace contracts
{	
	namespace common
	{
		void Logger::initialize()
		{
			if (initialized_)
				return;

			//boost::filesystem::path dir("logs/");
			auto path = std::experimental::filesystem::current_path();
			path.concat("\\logs\\");
			create_directory(path);

			loggers_.push_back(daily_logger_mt("daily_logger", "logs/daily", 2, 30));
			loggers_.push_back(stdout_logger_mt("console", true));
			set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");
			initialized_ = true;
		}

		std::list<std::shared_ptr<logger>> Logger::loggers_;
		bool Logger::initialized_;
	}
}