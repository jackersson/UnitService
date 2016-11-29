#include "raw_data.hpp"

#include <opencv2/opencv.hpp>

namespace video_device
{	
	RawImage::RawImage(const cv::Mat& data)
	{
		width_   = data.cols;
		height_  = data.rows;
		int size = data.total() * data.elemSize() * sizeof(unsigned char);
		data_    = std::unique_ptr<unsigned char[]>(new unsigned char[size]);
		std::memcpy(data_.get(), data.data, size);
	}	
}
