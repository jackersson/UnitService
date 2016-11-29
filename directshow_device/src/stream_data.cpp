#include "stream_data.hpp"
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgcodecs.hpp>

namespace video_device
{	
	std::vector<int> StreamData::jpeg_params = { CV_IMWRITE_JPEG_QUALITY, 100 };

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
}
