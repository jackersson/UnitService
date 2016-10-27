#ifndef TestableDatabaseImpl_Included
#define TestableDatabaseImpl_Included

#include <contracts/services/service_address.hpp>
#include <client_context.hpp>
#include <database_service/database_client_impl.hpp>
#include <data_api.hpp>
#include <repository_container.hpp>

namespace data_tests
{
	class TestableDatabaseImpl
	{
	public:
		explicit TestableDatabaseImpl(const std::string& address)	
		{		
			contracts::services::ServiceAddress database_address(address);
			grpc_services::ClientContext client_context(database_address, nullptr);

			impl_ = std::make_unique<grpc_services::DatabaseClientImpl>(client_context);
			impl_->start();

			api_.set_api(impl_.get());

			datacontext_ = std::make_unique<data_core::DataContextContainer>(api_);
			datacontext_->init();

			repositories_ 
				= std::make_unique<data_core::RepositoryContainer>(datacontext_.get());
			repositories_->init();
		}

		~TestableDatabaseImpl() {
			stop();
		}

		template <typename T>
		contracts::data::IRepository<T>* get(){
			return repositories_->get<T>();
		}

		void stop()	{
			impl_->stop();
		}

	private:
		std::unique_ptr<grpc_services::DatabaseClientImpl> impl_;
		data_core::DataApi api_;
		std::unique_ptr<contracts::data::AbstractDataContextContainer> datacontext_;
		std::unique_ptr<contracts::data::AbstractRepositoryContainer> repositories_;
	};
}

#endif
