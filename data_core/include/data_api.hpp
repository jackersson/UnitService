#ifndef DataApi_Included
#define DataApi_Included

#include <contracts/common/logger.hpp>
#include <contracts/services/idatabase_api.hpp>

namespace data_core
{
	class DataApi
	{
	public:
		DataApi()	: logger_(nullptr)
		        	, api_   (nullptr)
		{}

		void set_logger(contracts::common::LoggerPtr logger) 	{
			logger_ = logger;
		}

		void set_api(contracts::services::IDatabaseApi* d_api) {
			api_ = d_api;
		}

		contracts::common::LoggerPtr logger() const
		{
			if (logger_ == nullptr)
				logger_ = std::shared_ptr<contracts::common::Logger>();
			return logger_;
		}

		contracts::services::IDatabaseApi* api() const
		{
			if (api_ == nullptr)
				logger()->error("Null reference exception Database API");
			return api_;
		}

		bool empty() const
		{
			return api_ == nullptr;
		}

	private:
		mutable contracts::common::LoggerPtr logger_ ;
		contracts::services::IDatabaseApi*   api_    ;
	};
}

#endif
