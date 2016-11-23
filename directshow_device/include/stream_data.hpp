#ifndef StreamData_Included
#define StreamData_Included

#include <contracts/devices/video_device/istream_data.hpp>
#include <opencv2/core/mat.hpp>

namespace cv {
	class Mat;
}

namespace contracts {
	namespace devices {
		namespace video_device{
			enum StreamType;
		}
	}
}

namespace directshow_device
{
	
	class StreamData : public contracts::devices::video_device::IStreamData
	{
	public:
		StreamData(const cv::Mat& mat);
		/*
		contracts::devices::video_device::IRawImage*
			try_get_data(contracts::devices::video_device::StreamType type) const override;

		void add( contracts::devices::video_device::StreamType type
			       , cv::Mat& image);
		*/

	//	void set_mat(const cv::Mat& mat)	{
		//	mat_ = mat;
	//	}

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