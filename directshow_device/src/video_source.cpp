#include "video_source.hpp"

#include "opencv2/opencv.hpp"
#include "stream_data.hpp"
#include "directshow_device_info.hpp"

#include <data/models/devices.hpp>



namespace directshow_device
{

	VideoSource::VideoSource() : pause_(false)
		, video_capture_(std::make_unique<cv::VideoCapture>())
		, stream_data_(std::make_unique<StreamData>())
		, capture_error_fault_(0)
	{}

	bool VideoSource::open(int device_id)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		video_capture_->open(device_id);
		if (!video_capture_->isOpened())
			on_error(contracts::devices::DeviceException("Can't open device"
				, data_model::DeviceType::Capture));
		else
			capture_error_fault_ = 0;

		return is_open();
	}

	bool VideoSource::is_open() const
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		auto result = video_capture_ != nullptr
			? (video_capture_->isOpened() && !capture_error())
			: false;
		return result;
	}

	void VideoSource::release()
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		video_capture_->release();
	}

	void VideoSource::set_capability(const Capability& capability)
	{
		video_capture_->set(CV_CAP_PROP_FRAME_WIDTH, capability.width());
		video_capture_->set(CV_CAP_PROP_FRAME_HEIGHT, capability.height());
	}

	Capability VideoSource::get_capability() const
	{
		auto width = video_capture_->get(CV_CAP_PROP_FRAME_WIDTH);
		auto height = video_capture_->get(CV_CAP_PROP_FRAME_HEIGHT);

		return Capability(width, height);
	}

	void VideoSource::pause()
	{
		pause_ = true;
	}

	void VideoSource::play()
	{
		start();
		pause_ = false;
	}

	void VideoSource::run()  {

		while (true)
		{
			if (pause_ || !is_open())
				continue;

			cv::Mat frame;
			auto flag = video_capture_->retrieve(frame);
			if (flag)
			{
				capture_error_fault_ = 0;
				stream_data_->add(contracts::devices::video_device::StreamTypeColor
					, frame);

				for (auto observer : observers_)
					observer->on_next(*stream_data_);
			}
			else
			{
				on_error(contracts::devices::DeviceException("Capture error"
					, data_model::DeviceType::Capture));
				capture_error_fault_++;
			}
			if (cancelation_requested)
				break;
		}
	}

	void VideoSource::on_error(const contracts::devices::DeviceException& exception)
	{
		for (auto observer : observers_)
			observer->on_error(exception);
	}

	bool VideoSource::capture_error() const {
		return capture_error_fault_ > CAPTURE_ERROR_THRESHOLD;
	}
}
