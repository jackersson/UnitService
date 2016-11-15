#ifndef RawData_Included
#define RawData_Included

#include <memory>
#include <contracts/devices/video_device/istream_data.hpp>

namespace cv {
	class Mat;
}

namespace directshow_device
{
	class RawImage : public contracts::devices::video_device::IRawImage
	{
	public:
		explicit RawImage(const cv::Mat& data);

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
		std::unique_ptr<unsigned char[]> data_;
	};
}
#endif