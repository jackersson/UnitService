#ifndef AccessDeviceTypes_Included
#define AccessDeviceTypes_Included


namespace access_device
{	
	//only this can be managed by users
	enum lights
	{
		  None = 0
		, Pb1RedMain     = 1 << 1
		, Pb6Green       = 1 << 5
		, Pb7Light       = 1 << 6
		, Pb8GreenAccess = 1 << 7
	};
	
	enum buttons
	{
		  OpenedGate    = 0
		, Pc1Access     = 1 << 1
		, Pc2Reserve    = 1 << 2
		, Pc3Operator   = 1 << 3
		, Pc4ClosedGate = 1 << 4
	};
	
	enum access_device_module
	{
		  Lights  = 130
		, Buttons = 131
		, Dallas  = 132
	};
	
}

#endif