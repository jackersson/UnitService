#ifndef DirectShowDeviceListener_Included
#define DirectShowDeviceListener_Included

#include <queue>

#include <threadable.hpp>
#include <mutex>
#include <contracts/video/iplayer.hpp>
#include <directshow_device_info.hpp>
#include <video_source.hpp>
#include <contracts/devices/idevice_info.hpp>
#include <observers/observable.hpp>

namespace directshow_device
{

	class DirectshowDeviceListener  : public utils::Threadable
		     , public contracts::video::IMediaPlayer
		    // , public contracts::observers::Observable<IVideoDeviceObserver>
		     , public contracts::observers::IObservable<IVideoDeviceObserver>
		     //, public IVideoDeviceObserver
	{
	public:
		explicit DirectshowDeviceListener(std::string device_name
		, contracts::devices::IDeviceInfo<DirectShowDeviceInfo>* device_holder)
			: device_name_(device_name)
			, video_capture_(std::make_unique<VideoSource>())
			, device_connectivity_(device_holder)
		{
			video_capture_ = std::make_unique<VideoSource>();
			try_connect();
		}
		
		void try_kill  () override {
			commands_.push(contracts::video::PlayerCommands::Kill);
		}

		void try_stop  () override {
			commands_.push(contracts::video::PlayerCommands::Stop);
		}

		void try_start () override {
			commands_.push(contracts::video::PlayerCommands::Start);
		}

		void try_pause () override {
			commands_.push(contracts::video::PlayerCommands::Pause);
		}

		void try_resume() override {
			commands_.push(contracts::video::PlayerCommands::Resume);
		}

		void try_connect() {
			commands_.push(contracts::video::PlayerCommands::Connect);
		}

		void open( DirectShowDeviceInfo& device)
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

		bool is_active() const {
			return active();
		}

	protected:
		void run() override
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
					if (device_connectivity_->try_get_info(device_name_, finfo))
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

		void clear()
		{
			std::lock_guard<std::mutex> lock(mutex_);
			while (!commands_.empty())
				commands_.pop();
		}

		contracts::video::PlayerCommands dequeue()
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

public:
		void subscribe(IVideoDeviceObserver* observer) override	{
			video_capture_->subscribe(observer);
		}

		void unsubscribe(IVideoDeviceObserver* observer) override	{
			video_capture_->unsubscribe(observer);
		} 

		bool has_observer(IVideoDeviceObserver* observer) override {
			return video_capture_->has_observer(observer);
		}

		void unsubscribe_all() override	{
			video_capture_->unsubscribe_all();
		}

		size_t count() const override	{
			return video_capture_->count();
		}				
	private:

		DirectshowDeviceListener(const DirectshowDeviceListener& other) = delete;
		DirectshowDeviceListener& operator=(const DirectshowDeviceListener&) = delete;

		contracts::devices::IDeviceInfo<DirectShowDeviceInfo>* device_connectivity_;
		 std::unique_ptr<VideoSource> video_capture_;
		std::mutex mutex_;

		std::string device_name_;
		std::queue<contracts::video::PlayerCommands> commands_;
				
		std::chrono::milliseconds delay_between_commands_
			= std::chrono::milliseconds(0);

		std::chrono::milliseconds empty_command_timeout_
			= std::chrono::milliseconds(200);
	};

	typedef std::shared_ptr<DirectshowDeviceListener> DirectshowDeviceListenerPtr;

}

#endif
