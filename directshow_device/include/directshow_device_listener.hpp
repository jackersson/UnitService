#ifndef DirectShowDeviceListener_Included
#define DirectShowDeviceListener_Included

#include <queue>

#include <threadable.hpp>
#include <mutex>
#include <contracts/video/iplayer.hpp>
#include <contracts/devices/device_observer.hpp>
#include <contracts/devices/video_device/istream_data.hpp>
#include <contracts/observers/observable..hpp>

namespace directshow_device
{
	typedef contracts::devices::IDeviceObserver<contracts::devices::video_device::IStreamData> IVideoDeviceObserver;

	class DirectshowDeviceListener final : public utils::Threadable
		     , public contracts::video::IMediaPlayer
		     , public contracts::observers::Observable<IVideoDeviceObserver>
		     , public IVideoDeviceObserver
	{
	public:
		explicit DirectshowDeviceListener(std::string device_name)
			: device_name_(device_name)
		{}
		
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

		void open(int device_id)
		{
			//VideoCapture cap(0)

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
				//	DoStopStream();
			//		Stop();
					break;

				case contracts::video::PlayerCommands::Start:
				//	DoStart();
					break;

				case contracts::video::PlayerCommands::Stop:
				//	DoStopStream();
					break;

				case contracts::video::PlayerCommands::Connect:
				{
				////	FilterInfo filterInfo;
					//if (_devices.TryGetDeviceInfo(_deviceName, out filterInfo))
					//	OpenStream(filterInfo);
					//else
					//	TryConnect();
					break;
				}
				case contracts::video::PlayerCommands::Pause:
					//DoPause();
					break;
				case contracts::video::PlayerCommands::Resume:
					//DoStart();
					break;
				case contracts::video::None: break;
				default: break;
				}
				std::this_thread::sleep_for(delay_between_commands_);
			}
		}
			
		bool is_active() const
		{
			return active();
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
				std::this_thread::sleep_for(empty_command_timeout_);

			auto result = commands_.front();
			commands_.pop();
			return result;  
		}

	private:

		DirectshowDeviceListener(const DirectshowDeviceListener& other) = delete;
		DirectshowDeviceListener& operator=(const DirectshowDeviceListener&) = delete;


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
