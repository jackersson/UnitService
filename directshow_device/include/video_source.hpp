#ifndef VideoSource_Included
#define VideoSource_Included

#include <memory>

#include <observers/observable.hpp>
#include <contracts/devices/device_observer.hpp>
#include <contracts/devices/video_device/istream_data.hpp>

//#include "std_threadable.hpp"

#include <threadable.hpp>

namespace cv {
	class VideoCapture;
}

namespace directshow_device
{	
	class StreamData;
	class Capability;
	class DirectShowDeviceInfo;
	
	class VideoSource : public utils::Threadable
		, public contracts::observers::Observable
		            <contracts::devices::IDeviceObserver
		            <contracts::devices::video_device::IStreamData>>
	{
	public:
		VideoSource();
		bool open(int device_id);

		bool is_open() const;

		void release();

		void set_capability(const Capability& capability);

		Capability get_capability() const;

		void pause();
		void play ();

	protected:
		void run() override;

	private:

		void on_error(const contracts::devices::DeviceException& exception);
		bool capture_error() const;

		VideoSource(const VideoSource& other) = delete;
		VideoSource& operator=(const VideoSource&) = delete;

		bool pause_;
		std::unique_ptr<cv::VideoCapture> video_capture_;
		std::unique_ptr<StreamData>       stream_data_  ;
		int  capture_error_fault_;

		mutable std::recursive_mutex mutex_;

		const int CAPTURE_ERROR_THRESHOLD = 3;
	};

}

#endif
