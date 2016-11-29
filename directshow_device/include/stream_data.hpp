#ifndef StreamData_Included
#define StreamData_Included

#include <devices/video_device/istream_data.hpp>
#include <opencv2/core/mat.hpp>

namespace contracts {
	namespace devices {
		namespace video_device{
			enum StreamType;
		}
	}
}

namespace video_device
{	
	class StreamData : public IStreamData
	{
	public:
		explicit StreamData(const cv::Mat& mat);
	
		const cv::Mat& color() const override{
			return mat_;
		}

		const std::vector<unsigned char>& jpeg_bytes() const override;

	private:
		mutable std::vector<unsigned char> jpeg_;
		cv::Mat mat_;

		static std::vector<int> jpeg_params;
		//std::unique_ptr<contracts::devices::video_device::IRawImage> color_;
	};
}
#endif