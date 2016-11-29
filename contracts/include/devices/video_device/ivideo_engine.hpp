#ifndef IVideoEngine_Included
#define IVideoEngine_Included

#include "istream_data.hpp"
#include "devices/idevice_engine.hpp"

namespace video_device
{	
	class IVideoEngine : public locations::IDeviceEngine<IStreamDataPtr>
	{
	public:
		virtual ~IVideoEngine() {}
		
	};
}

#endif
