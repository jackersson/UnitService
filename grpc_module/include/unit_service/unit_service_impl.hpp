#ifndef UnitServiceImpl_INCLUDED
#define UnitServiceImpl_INCLUDED

#include <contracts/iservice_context.hpp>
#include <async_service_base.hpp>
#include <services/unit_service.grpc.pb.h>

namespace grpc_services
{
	namespace unit_service
	{		
		class UnitServiceImpl 
			: public services_api::AsyncServiceBase<Services::UnitService::AsyncService>
		{
		public:
			explicit UnitServiceImpl( contracts::services::IServiceAddress& address
				                      , grpc::ServerBuilder*        server_builder
		  	                      , contracts::IServiceContext* unit_context  );
				     
			virtual ~UnitServiceImpl();

			void de_init() override;
			void init   () override;		

		private:
			template <typename T>
			void initialize_handler()
			{
				add_rpc_handler    <T>();
				add_handler_creator<T>();
			}
			
			template <typename T>
			void add_handler_creator()
			{
				services_api::CreateRequestHandlerFunc func 
					= [this](grpc::ServerCompletionQueue* queue)
				{
					T::create(&service_, queue, unit_context_);
				};			
				add_handler_factory_method<T>(func);				
			}
		
			std::string class_name() const override {
				return typeid(UnitServiceImpl).name();
			}

			contracts::IServiceContext* unit_context_;

			UnitServiceImpl(const UnitServiceImpl&) = delete;
			UnitServiceImpl& operator=(const UnitServiceImpl&) = delete;
		};
	}
}

#endif