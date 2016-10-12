#ifndef Logger_Included
#define Logger_Included

#include <spdlog/spdlog.h>

namespace contracts
{
	namespace common
	{
		class Logger
		{
		public:
			Logger() {
				initialize();
			}

			template <typename... Args>
			void debug(const char* fmt, const Args&... args)
			{
				for (auto logger : loggers_)
					logger->debug(fmt, args);
			}

			template<typename T>
			void debug(const T& msg)
			{
				for (auto logger : loggers_)
					logger->debug(msg);
			}

			template <typename... Args>
			void trace(const char* fmt, const Args&... args)
			{
				for (auto logger : loggers_)
					logger->trace(fmt, args);
			}

			template<typename T>
			void trace(const T& msg)
			{
				for (auto logger : loggers_)
					logger->trace(msg);
			}

			template <typename... Args>
			void warn(const char* fmt, const Args&... args)
			{
				for (auto logger : loggers_)
					logger->warn(fmt, args);
			}

			template<typename T>
			void warn(const T& msg)
			{
				for (auto logger : loggers_)
					logger->warn(msg);
			}

			template <typename... Args>
			void critical(const char* fmt, const Args&... args)
			{
				for (auto logger : loggers_)
					logger->critical(fmt, args);
			}

			template<typename T>
			void critical(const T& msg)
			{
				for (auto logger : loggers_)
					logger->critical(msg);
			}

			template <typename... Args>
			void error(const char* fmt, const Args&... args)
			{
				for (auto logger : loggers_)
					logger->error(fmt, args);
			}

			template<typename T>
			void error(const T& msg)
			{
				for (auto logger : loggers_)
					logger->critical(msg);
			}
			/*
			template <typename... Args>
			void info(const char* fmt, const Args&... args)
			{
				for (auto logger : loggers_)
					logger->info(fmt, args);
			}
			*/
			template<typename T>
			void info(const T& msg)
			{
				for (auto logger : loggers_)
					logger->info(msg);
			}

		private:
			static void initialize();		

			static std::list<std::shared_ptr<spdlog::logger>> loggers_;
			static bool initialized_;

		};
	}
}

#endif

