#include "stream_data.hpp"

namespace directshow_device
{	
	contracts::devices::video_device::IRawImage*
		StreamData::try_get_data(contracts::devices::video_device::StreamType type) const 
	{
		switch (type)
		{
		case contracts::devices::video_device::StreamTypeColor:
			return color_.get();
		case contracts::devices::video_device::StreamTypeDepth: break;
		default: break;
		}
		return nullptr;
	}

	void StreamData::add( contracts::devices::video_device::StreamType type
		                  , cv::Mat& image)
	{
		switch (type)
		{
		case contracts::devices::video_device::StreamTypeColor:
			color_ = std::make_unique<RawImage>(image);
			break;
		case contracts::devices::video_device::StreamTypeDepth: break;
		default: break;
		}
	}
}
