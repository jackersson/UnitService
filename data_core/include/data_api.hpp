#ifndef DataApi_Included
#define DataApi_Included

#include <datatypes/key.pb.h>
#include <contracts/common/logger.hpp>
#include <contracts/services/idatabase_api.hpp>

namespace data_core
{
	class DataApi
	{
	public:
		void set_logger(contracts::common::LoggerPtr logger) 	{
			logger_ = logger;
		}

		void set_api(contracts::services::IDatabaseApiPtr d_api) {
			api_ = d_api;
		}

		contracts::common::LoggerPtr logger() const
		{
			return logger_;
		}

		contracts::services::IDatabaseApiPtr api() const
		{
			if (api_ == nullptr)
				logger_->error("Null reference exception Database API");
			return api_;
		}

	private:
		contracts::common::LoggerPtr         logger_  ;
		contracts::services::IDatabaseApiPtr api_;
	};
}

#endif
