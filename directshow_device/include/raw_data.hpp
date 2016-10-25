#ifndef RawData_Included
#define RawData_Included

#include <memory>
#include <contracts/devices/video_device/istream_data.hpp>
#include "opencv2/opencv.hpp"

namespace directshow_device
{
	class RawImage : public contracts::devices::video_device::IRawImage
	{
	public:
		explicit RawImage(const cv::Mat& data)
		{
			width_   = data.cols;
			height_  = data.rows;
			int size = data.total() * data.elemSize() * sizeof(unsigned char);
			data_ = std::unique_ptr<unsigned char>(new unsigned char[size]);
			std::memcpy(data_.get(), data.data, size);
		}
		virtual ~RawImage() {}

		bool empty() const {
			return width_ * height_ <= 0;
		}

		int width() const override	{
			return width_;
		}
		int height() const override	{
			return height_;
		}

		unsigned char* data() const override {
			return data_.get();
		}

	private:
		int width_;
		int height_;
		std::unique_ptr<unsigned char> data_;
	};
}
#endif