#ifndef AccessDeviceTypes_Included
#define AccessDeviceTypes_Included

namespace contracts
{
	namespace devices
	{
		namespace access_device
		{
			//only this can be managed by users
			enum lights
			{
				  lNone        = 0
				, lRedMain     = 1 << 1
				, lGreen       = 1 << 5
				, lLight       = 1 << 6
				, lGreenAccess = 1 << 7
			};

			enum buttons
			{
				  bOpenedGate    = 0
				, bAccess        = 1 << 1
				, bPc2Reserve    = 1 << 2
				, bPc3Operator   = 1 << 3
				, bPc4ClosedGate = 1 << 4
			};

			enum access_device_module
			{
				  NoneModule = 0
			  ,	Lights  = 130
				, Buttons = 131
				, Dallas  = 132
			};
		}
	}
}

#endif