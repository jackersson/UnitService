#include "stream_data.hpp"
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgcodecs.hpp>

namespace directshow_device
{	
	std::vector<int> StreamData::jpeg_params = { CV_IMWRITE_JPEG_QUALITY, 50 };

	StreamData::StreamData(const cv::Mat& mat)
		:mat_(mat)
	{
		
	}

	//TODO pass parametrs
	const std::vector<unsigned char>& StreamData::jpeg_bytes() const
	{
		if (jpeg_.size() > 0)
			return jpeg_;	

		imencode(".jpg", mat_, jpeg_, jpeg_params);
		return jpeg_;
	}

	/*
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
	*/
}
