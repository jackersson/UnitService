#ifndef Logger_Included
#define Logger_Included

#include "spdlog/spdlog.h"

namespace logging
{
	class Logger
	{
	public:		
		Logger() {
			initialize();
		}

		template <typename... Args>
		inline void debug(const char* fmt, const Args&... args)
		{
			for (auto logger : loggers_)
				logger->debug(fmt, args);
		}

		template<typename T>
		inline void debug(const T& msg)
		{
			for (auto logger : loggers_)
				logger->debug(msg);
		}

		template <typename... Args>
		inline void trace(const char* fmt, const Args&... args)
		{
			for (auto logger : loggers_)
				logger->trace(fmt, args);
		}

		template<typename T>
		inline void trace(const T& msg)
		{
			for (auto logger : loggers_)
				logger->trace(msg);
		}

		template <typename... Args>
		inline void warn(const char* fmt, const Args&... args)
		{
			for (auto logger : loggers_)
				logger->warn(fmt, args);
		}

		template<typename T>
		inline void warn(const T& msg)
		{
			for (auto logger : loggers_)
				logger->warn(msg);
		}

		template <typename... Args>
		inline void critical(const char* fmt, const Args&... args)
		{
			for (auto logger : loggers_)
				logger->critical(fmt, args);
		}

		template<typename T>
		inline void critical(const T& msg)
		{
			for (auto logger : loggers_)
				logger->critical(msg);
		}

		template <typename... Args>
		inline void error(const char* fmt, const Args&... args)
		{
			for (auto logger : loggers_)
				logger->error(fmt, args);
		}

		template<typename T>
		inline void error(const T& msg)
		{
			for (auto logger : loggers_)
				logger->critical(msg);
		}

		template <typename... Args>
		inline void info(const char* fmt, const Args&... args)
		{
			for (auto logger : loggers_)
				logger->info(fmt, args);
		}

		template<typename T>
		inline void info(const T& msg)
		{
			for (auto logger : loggers_)
				logger->info(msg);
		}

	private:
		static void initialize()
		{
			if (initialized_)
				return;

			loggers_.push_back(spdlog::daily_logger_mt("daily_logger", "logs/daily", 2, 30));
			loggers_.push_back(spdlog::stdout_logger_mt("console", true));
			initialized_ = true;
		}

		static std::list<std::shared_ptr<spdlog::logger>> loggers_;
		static bool initialized_;

	};
}

#endif

