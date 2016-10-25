#ifndef StreamData_Included
#define StreamData_Included

#include <contracts/devices/video_device/istream_data.hpp>
#include "raw_data.hpp"

namespace directshow_device
{
	class StreamData : public contracts::devices::video_device::IStreamData
	{
	public:

		virtual ~StreamData() {}

		contracts::devices::video_device::IRawImage*
			try_get_data(contracts::devices::video_device::StreamType type) const override
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

		void add(contracts::devices::video_device::StreamType type
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

	private:
		std::unique_ptr<contracts::devices::video_device::IRawImage> color_;
	};
}
#endif