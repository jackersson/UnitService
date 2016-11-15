#ifndef Rs232ControllerTypes_Included
#define Rs232ControllerTypes_Included

namespace access_device
{
	namespace rs232
	{
		const int READ_COMMAND_SIZE  = 5;
		const int WRITE_COMMAND_SIZE = 7;

		const int DALLAS_SIZE     = 17; //whole command size
		const int DALLAS_KEY_SIZE = 6 ; //only data key size

		enum port_b_commands
		{
			  None           = 0
			, Pb0RedPulsed   = 1
			, Pb1RedMain     = 1 << 1
			, Pb2Empty       = 1 << 2
			, Pb3Empty       = 1 << 3
			, Pb4Empty       = 1 << 4
			, Pb6Green       = 1 << 5
			, Pb7Light       = 1 << 6
			, Pb8GreenAccess = 1 << 7
		};

		enum port_c_response
		{
			  ClosedGate  = 0
			, Pc0Empty    = 1
			, Pc1Access   = 1 << 1
			, Pc2Reserve  = 1 << 2
			, Pc3Operator = 1 << 3
			, Pc4OpenGate = 1 << 4
			, Pc6Empty    = 1 << 5
			, Pc7Empty    = 1 << 6
			, Pc8Empty    = 1 << 7
		};

		enum port_command : unsigned char
		{
			  NonePortCommand = 0
			, ReadB           = 129
			, WriteB          = 130
			, ReadC           = 131
			, Dallas          = 132
			, Master          = 133
		};
	}
}

#endif
