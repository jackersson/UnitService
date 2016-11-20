#ifndef DirectShowDeviceObserver_Included
#define DirectShowDeviceObserver_Included
#include <directshow_device_engine.hpp>
#include <opencv/cv.hpp>


namespace directshow_tests
{
	inline 
	cv::Mat bytes_to_mat(unsigned char* bytes, int width, int height)
	{
		auto image = cv::Mat(height, width, CV_8UC3, bytes);
		return image;
	}

	class DirectshowDeviceObserver : public directshow_device::IVideoDeviceObserver
	{
	public:
		DirectshowDeviceObserver()
		{

		}

		void on_error(const contracts::devices::DeviceException& exception) override
		{
			std::cout << exception.what() << std::endl;
		}

		void on_state(const contracts::devices::IDeviceState& state) override
		{
			//cout << state.state() << endl;
		}

		void on_next(const contracts::devices::video_device::IStreamData& data) override
		{
			std::cout << "frame got" << std::endl;


			auto im = data.try_get_data(contracts::devices::video_device::StreamTypeColor);
			if (im == nullptr)
				return;

			auto frame = bytes_to_mat(im->data(), im->width(), im->height());
			cv::imshow("image", frame);
			if (cvWaitKey(10) >= 0)
			{
				return;
			}
		}

	};
}

#endif
