#ifndef UnitServiceImpl_INCLUDED
#define UnitServiceImpl_INCLUDED

#include <services/service_address.hpp>
#include <helpers/service_heplers.hpp>

#include "check_device_request_handler.hpp"
#include "enroll_request_handler.hpp"
#include "get_card_request_handler.hpp"
#include "get_device_stream_request_handler.hpp"
#include "get_location_stream_request_handler.hpp"
#include "open_door_request_handler.hpp"
#include "update_location_request_handler.hpp"

#include <contracts/iservice_context.hpp>
#include <async_service_base.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		class UnitServiceImpl 
			: public services_api::AsyncServiceBase<services_api::AsyncUnitService>
		{
		public:
			explicit UnitServiceImpl( contracts::services::IServiceAddress& address
				                      , grpc::ServerBuilder*        server_builder
		  	                      , contracts::IServiceContext* unit_context  )
				                      : AsyncServiceBase(address, server_builder)
				                      , unit_context_(unit_context)
			{
				UnitServiceImpl::init();
			}

			virtual ~UnitServiceImpl() {
				UnitServiceImpl::de_init();
			}

			void de_init() override {
				stop();
			}

			void init() override
			{
				connect();

				initialize_handler<CheckDeviceRequestHandler      >();
				initialize_handler<EnrollRequestHandler           >();
				initialize_handler<GetCardRequestHandler          >();
				initialize_handler<GetDeviceStreamRequestHandler  >();
				initialize_handler<GetLocationStreamRequestHandler>();
				initialize_handler<OpenDoorRequestHandler         >();
				initialize_handler<UpdateLocationRequestHandler   >();
			}

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
				//auto func = std::bind( T::Create            , &service_
					  //                 , std::placeholders::_1, unit_context_ );
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