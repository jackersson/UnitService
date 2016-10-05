#ifndef Rs232ControllerTypes_Included
#define Rs232ControllerTypes_Included

namespace access_device
{
	namespace rs232
	{
		enum port_b_commands
		{
			None = 0
			, Pb0RedPulsed = 1
			, Pb1RedMain = 1 << 1
			, Pb2Empty = 1 << 2
			, Pb3Empty = 1 << 3
			, Pb4Empty = 1 << 4
			, Pb6Green = 1 << 5
			, Pb7Light = 1 << 6
			, Pb8GreenAccess = 1 << 7
		};

		enum port_c_response
		{
			ClosedGate = 0
			, Pc0Empty = 1
			, Pc1Access = 1 << 1
			, Pc2Reserve = 1 << 2
			, Pc3Operator = 1 << 3
			, Pc4OpenGate = 1 << 4
			, Pc6Empty = 1 << 5
			, Pc7Empty = 1 << 6
			, Pc8Empty = 1 << 7
		};

		enum port_command
		{
			ReadB = 129
			, WriteB = 130
			, ReadC = 131
			, Dallas = 132
			, Master = 133
		};

		enum command_type
		{
			NoneCommand = 0
			, Read
			, Write
		};
	}
}

#endif
