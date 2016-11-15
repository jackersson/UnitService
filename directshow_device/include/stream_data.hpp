#ifndef StreamData_Included
#define StreamData_Included

#include <contracts/devices/video_device/istream_data.hpp>
#include "raw_data.hpp"

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
		contracts::devices::video_device::IRawImage*
			try_get_data(contracts::devices::video_device::StreamType type) const override;

		void add( contracts::devices::video_device::StreamType type
			       , cv::Mat& image);

	private:
		std::unique_ptr<contracts::devices::video_device::IRawImage> color_;
	};
}
#endif