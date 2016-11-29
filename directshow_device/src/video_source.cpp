#include "video_source.hpp"

#include "opencv2/opencv.hpp"
#include "stream_data.hpp"
#include "directshow_device_info.hpp"

#include <data/models/devices.hpp>

using namespace devices   ; 
using namespace data_model;


namespace video_device
{
	VideoSource::VideoSource() 
		: pause_(false)
		, video_capture_(std::make_unique<cv::VideoCapture>())
		, capture_error_fault_(0)
		, busy_(false)
	{}

	VideoSource::~VideoSource()
	{
		release();
	}

	bool VideoSource::open(int device_id)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		video_capture_->open(device_id);
		if (!video_capture_->isOpened())
			on_error(DeviceException("Can't open device", Capture));
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

				auto dt = std::make_shared<StreamData>(frame);
				on_next(dt);
			}
			else
			{
				on_error(DeviceException("Capture error", Capture));
				capture_error_fault_++;
			}
			if (cancelation_requested)
				break;
		}

		release();
	}

	void VideoSource::on_error(const DeviceException& exception)
	{
		notify([&exception, this](std::vector<IDeviceObserver
			<IStreamDataPtr>*>& observers)
		{
			//for (auto observer : observers)
			//{
				//TODO make task
				//if (observer != nullptr)
				//	observer->on_error(exception);
		//	}
		});
	}

	void VideoSource::on_next(IStreamDataPtr data)
	{			
	
		notify([data, this](std::vector<IDeviceObserver
			<IStreamDataPtr>*>& observers)
		{
			if (observers.size() <= 0)			
				return;
		
	#pragma omp parallel for
			for (auto i = 0; i < observers.size(); ++i)
				observers[i]->on_next(data);			
		});
	}

	void VideoSource::on_state(const IDeviceState&)
	{
		//Not implemented
	}

	bool VideoSource::capture_error() const {
		return capture_error_fault_ > CAPTURE_ERROR_THRESHOLD;
	}
}
