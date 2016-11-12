#ifndef IAccessCoordinator_Included
#define IAccessCoordinator_Included

namespace data_model
{
	class AccessDevice ;
	enum  AccessState  ;
	class CaptureDevice;
}

namespace contracts
{
	namespace devices
	{
		namespace access_device
		{
			class IAccessDeviceUpdatable
			{
			public:
				virtual ~IAccessDeviceUpdatable() {}

				virtual void update(const data_model::AccessDevice& device) = 0;
			};

			class ICaptureDeviceUpdatable
			{
			public:
				virtual ~ICaptureDeviceUpdatable() {}

				virtual void update(const data_model::CaptureDevice& device) = 0;
			};

			class IAccessCoordinator: public IAccessDeviceUpdatable
				                      , public common::ILifecycle
			{
			public:
				virtual ~IAccessCoordinator() {}
		
				virtual void set_state(data_model::AccessState  state) const = 0;
			};

		}
	}
}

#endif
