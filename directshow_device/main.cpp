#include <iostream>
#include <thread>
#include "directshow_device_engine.hpp"
using namespace std;
using namespace cv;
using namespace contracts::devices::video_device;

Mat bytes_to_mat(unsigned char* bytes, int width, int height)
{
	auto image = Mat(height, width, CV_8UC3, bytes);
	return image;
}

class DirectshowDeviceObserver : public IVideoDeviceObserver
{
public:
	DirectshowDeviceObserver()
	{
		
	}

	void on_error(const contracts::devices::DeviceException& exception) override
	{
		cout << exception.what() << endl;
	}

	void on_state(const contracts::devices::IDeviceState& state) override
	{
		//cout << state.state() << endl;
	}

	void on_next(const IStreamData& data) override
	{
		cout << "frame got" << endl;


		auto im = data.try_get_data(StreamTypeColor);
		if (im == nullptr)
			return;

		auto frame = bytes_to_mat(im->data(), im->width(), im->height());
		imshow("image", frame);
		if (cvWaitKey(10) >= 0)
		{
			return;
		}
	}

};

int main()
{
	directshow_device::DirectShowDeviceEngine enumerator;
	DirectshowDeviceObserver obse;
	//this_thread::sleep_for(chrono::seconds(1));
	string camera_name = "LightCam 1020";

	enumerator.add(camera_name);
	enumerator.subscribe(&obse, camera_name);

	//this_thread::sleep_for(chrono::seconds(10));

	cout << "works" << endl;

	cin.get();
	enumerator.de_init();
	
	return 0;
}