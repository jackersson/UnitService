#ifndef IVideoEngine_Included
#define IVideoEngine_Included
#include "istream_data.hpp"
#include "contracts/devices/idevice_engine.hpp"

namespace contracts
{
	namespace devices
	{
		namespace video_device
		{
			
			class IVideoEngine : public locations::IDeviceEngine<IStreamData>
			{
			public:
				virtual ~IVideoEngine() {}
				
			};
		}
	}
}

#endif
