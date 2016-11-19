#include "unit_service/unit_service_impl.hpp"

#include "unit_service/check_device_request_handler.hpp"
#include "unit_service/enroll_request_handler.hpp"
#include "unit_service/get_card_request_handler.hpp"
#include "unit_service/get_device_stream_request_handler.hpp"
#include "unit_service/get_location_stream_request_handler.hpp"
#include "unit_service/open_door_request_handler.hpp"
#include "unit_service/update_location_request_handler.hpp"
#include <unit_service/get_devices_request_handler.hpp>

namespace grpc_services
{
	namespace unit_service
	{
		UnitServiceImpl::UnitServiceImpl(contracts::services::IServiceAddress& address
			, grpc::ServerBuilder*        server_builder
			, contracts::IServiceContext* unit_context)
			: AsyncServiceBase(address, server_builder)
			, unit_context_(unit_context)
		{
			UnitServiceImpl::init();
		}

		UnitServiceImpl::~UnitServiceImpl() {
			UnitServiceImpl::de_init();
		}

		void UnitServiceImpl::de_init() {
			stop();
		}

		void UnitServiceImpl::init()
		{
			connect();

			initialize_handler<CheckDeviceRequestHandler      >();
			initialize_handler<GetDevicesRequestHandler       >();
			initialize_handler<EnrollRequestHandler           >();
			initialize_handler<GetCardRequestHandler          >();
			initialize_handler<GetDeviceStreamRequestHandler  >();
			initialize_handler<GetLocationStreamRequestHandler>();
			initialize_handler<OpenDoorRequestHandler         >();
			initialize_handler<UpdateLocationRequestHandler   >();
		}
	}
}
