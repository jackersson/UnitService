#include "directshow_device_listener.hpp"

#include "directshow_device_info.hpp"
#include "video_source.hpp"

#include <data/models/devices.hpp>
#include <devices/idevice_info.hpp>

using namespace video  ;
using namespace devices;

namespace video_device
{
	DirectshowDeviceListener::DirectshowDeviceListener
   	( const std::string& device_name
		, IDeviceInfo<DirectShowDeviceInfo>* device_holder)
		: device_connectivity_(device_holder)
		, video_capture_(std::make_unique<VideoSource>())
		, device_name_(device_name)
	{
		try_connect();
	}		
	DirectshowDeviceListener::~DirectshowDeviceListener()
	{
		Threadable::stop();
	}


	void DirectshowDeviceListener::open(DirectShowDeviceInfo& device)
	{
		video_capture_->release();
		video_capture_->open(device.id());
		if (!video_capture_->is_open())
		{
			//on_error(contracts::devices::DeviceException("Can't open device"
			//	                                          , DataTypes::DeviceType::Capture));
			return;
		}

		auto best_capability = device.best_capability();
		video_capture_->set_capability(best_capability);
		video_capture_->play();
	}

	bool DirectshowDeviceListener::is_active() const {
		return active();
	}

	void DirectshowDeviceListener::run()
	{
		while (active())
		{
			auto command = dequeue();

			switch (command)
			{
			case Kill:
				video_capture_->release();
				break;

			case Start:
				video_capture_->play();
				break;

			case Stop:
				video_capture_->pause();
				break;

			case Connect:
			{
				DirectShowDeviceInfo finfo;
				if (device_connectivity_->try_get_device
				(data_model::DeviceId(device_name_), finfo))
					open(finfo);
				else
					try_connect();
				break;
			}
			case Pause:
				video_capture_->pause();
				break;
			case Resume:
				video_capture_->play();
				break;
			case None: break;
			default: break;
			}
			
			if  (cancelation_requested)
			{
				video_capture_->stop();
				break;
			}

			std::this_thread::sleep_for(delay_between_commands_);
		}
	}

	void DirectshowDeviceListener::clear()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		while (!commands_.empty())
			commands_.pop();
	}

	PlayerCommands DirectshowDeviceListener::dequeue()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		while (commands_.empty())
		{
			std::this_thread::sleep_for(empty_command_timeout_);
			if (!video_capture_->is_open())
			{
				try_connect();
				std::cout << "Video source disconnected" << std::endl;
			}

			if (cancelation_requested)
				return None;
		}

		auto result = commands_.front();
		commands_.pop();
		return result;
	}

	void DirectshowDeviceListener::subscribe(IDeviceObserver<IStreamDataPtr>* observer)  {
		video_capture_->subscribe(observer);
	}

	void DirectshowDeviceListener::unsubscribe(IDeviceObserver<IStreamDataPtr>* observer)  {
		video_capture_->unsubscribe(observer);
	}

	bool DirectshowDeviceListener::has_observer(IDeviceObserver<IStreamDataPtr>* observer)  {
		return video_capture_->has_observer(observer);
	}

	void DirectshowDeviceListener::unsubscribe_all() {
		video_capture_->unsubscribe_all();
	}

	size_t DirectshowDeviceListener::count() const {
		return video_capture_->count();
	}
}

