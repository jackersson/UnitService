#ifndef VideoSource_Included
#define VideoSource_Included

#include <memory>
#include <contracts/devices/video_device/istream_data.hpp>
#include "std_threadable.hpp"
#include "opencv2/opencv.hpp"
#include "directshow_device_info.hpp"
#include <observers/observable.hpp>
#include <types.hpp>
#include "stream_data.hpp"

namespace directshow_device
{	
	class VideoSource : public utils::StdThreadable
		                , public contracts::observers::Observable<IVideoDeviceObserver>
	{
	public:
		VideoSource() : pause_(false)
			, video_capture_(std::make_unique<cv::VideoCapture>())
			, stream_data_(std::make_unique<StreamData>())
			, capture_error_fault_(0)
		{}

		bool open(int device_id)
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

		bool is_open() const
		{
			std::lock_guard<std::recursive_mutex> lock(mutex_);
			auto result = video_capture_ != nullptr 
				          ? (video_capture_->isOpened() && !capture_error()) 
				          : false;
			return result;
		}

		void release()
		{
			std::lock_guard<std::recursive_mutex> lock(mutex_);
			video_capture_->release();
		}

		void set_capability(Capability& capability)
		{
			video_capture_->set(CV_CAP_PROP_FRAME_WIDTH , capability.width());
			video_capture_->set(CV_CAP_PROP_FRAME_HEIGHT, capability.height());
		}

		Capability get_capability() const
		{
			auto width  = video_capture_->get(CV_CAP_PROP_FRAME_WIDTH);
			auto height = video_capture_->get(CV_CAP_PROP_FRAME_HEIGHT);

			return Capability(width, height);
		}

		void pause()
		{
			pause_ = true;
		}

		void play()
		{
			start();
			pause_ = false;
		}

	protected:
		void run() override {
			
			while (true)
			{
				if (pause_ || !is_open())
					continue;

				cv::Mat frame;
				auto flag = video_capture_->retrieve(frame);
				if (flag)
				{
					capture_error_fault_ = 0;
					stream_data_->add( contracts::devices::video_device::StreamTypeColor
						               , frame);
					 
					for (auto observer : observers_)
						observer->on_next(*stream_data_);
				}
				else
				{					
					on_error( contracts::devices::DeviceException("Capture error"
							    , data_model::DeviceType::Capture));
					capture_error_fault_++;
				}
				if (cancelation_requested)
					break;
			}			
		}


	private:

		void on_error(const contracts::devices::DeviceException& exception)
		{
			for (auto observer : observers_)
				observer->on_error(exception);
		}

		bool capture_error() const	{
			return capture_error_fault_ > CAPTURE_ERROR_THRESHOLD;
		}

		VideoSource(const VideoSource& other) = delete;
		VideoSource& operator=(const VideoSource&) = delete;

		bool pause_;
		std::unique_ptr<cv::VideoCapture> video_capture_;
		std::unique_ptr<StreamData> stream_data_;
		int  capture_error_fault_;

		mutable std::recursive_mutex mutex_;

		const int CAPTURE_ERROR_THRESHOLD = 3;
	};

}

#endif
