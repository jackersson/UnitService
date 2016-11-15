#include "directshow_device_listener.hpp"

#include "directshow_device_info.hpp"
#include "video_source.hpp"

#include <data/models/devices.hpp>

namespace directshow_device
{
	DirectshowDeviceListener::DirectshowDeviceListener
   	( const std::string& device_name
		, contracts::devices::IDeviceInfo<DirectShowDeviceInfo>* device_holder)
		: device_connectivity_(device_holder)
		, video_capture_(std::make_unique<VideoSource>())
		, device_name_(device_name)
	{
		video_capture_ = std::make_unique<VideoSource>();
		try_connect();
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
			case contracts::video::PlayerCommands::Kill:
				video_capture_->release();
				break;

			case contracts::video::PlayerCommands::Start:
				video_capture_->play();
				break;

			case contracts::video::PlayerCommands::Stop:
				video_capture_->pause();
				break;

			case contracts::video::PlayerCommands::Connect:
			{
				DirectShowDeviceInfo finfo;
				if (device_connectivity_->try_get_device
				(data_model::DeviceId(device_name_), finfo))
					open(finfo);
				else
					try_connect();
				break;
			}
			case contracts::video::PlayerCommands::Pause:
				video_capture_->pause();
				break;
			case contracts::video::PlayerCommands::Resume:
				video_capture_->play();
				break;
			case contracts::video::None: break;
			default: break;
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

	contracts::video::PlayerCommands DirectshowDeviceListener::dequeue()
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
		}

		auto result = commands_.front();
		commands_.pop();
		return result;
	}

	void DirectshowDeviceListener::subscribe(IVideoDeviceObserver* observer)  {
		video_capture_->subscribe(observer);
	}

	void DirectshowDeviceListener::unsubscribe(IVideoDeviceObserver* observer)  {
		video_capture_->unsubscribe(observer);
	}

	bool DirectshowDeviceListener::has_observer(IVideoDeviceObserver* observer)  {
		return video_capture_->has_observer(observer);
	}

	void DirectshowDeviceListener::unsubscribe_all() {
		video_capture_->unsubscribe_all();
	}

	size_t DirectshowDeviceListener::count() const {
		return video_capture_->count();
	}
}

