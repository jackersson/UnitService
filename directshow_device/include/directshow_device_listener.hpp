#ifndef DirectShowDeviceListener_Included
#define DirectShowDeviceListener_Included

#include <queue>
#include <threadable.hpp>
#include <mutex>
#include <video/iplayer.hpp>
#include <devices/idevice_info.hpp>
#include <observers/observable.hpp>
#include <devices/video_device/istream_data.hpp>
#include <devices/device_observer.hpp>

namespace video_device
{
	class VideoSource;
	class Capability;
	class DirectShowDeviceInfo;

	class DirectshowDeviceListener  
		     : public utils::Threadable
		     , public video::IMediaPlayer
		     , public contracts::observers::IObservable
		              <devices::IDeviceObserver<IStreamDataPtr>>
	{
	public:
		explicit 
			DirectshowDeviceListener
			( const std::string& device_name
			, devices::IDeviceInfo<DirectShowDeviceInfo>* device_holder);
		
		~DirectshowDeviceListener();

		void try_kill  () override {
			commands_.push(video::PlayerCommands::Kill);
		}

		void try_stop  () override {
			commands_.push(video::PlayerCommands::Stop);
		}

		void try_start () override {
			commands_.push(video::PlayerCommands::Start);
		}

		void try_pause () override {
			commands_.push(video::PlayerCommands::Pause);
		}

		void try_resume() override {
			commands_.push(video::PlayerCommands::Resume);
		}

		void try_connect() {
			commands_.push(video::PlayerCommands::Connect);
		}

		void open(DirectShowDeviceInfo& device);

		bool is_active() const;

	protected:
		void run  () override;
		void clear();

		video::PlayerCommands dequeue();

public:
	void subscribe   (devices::IDeviceObserver<IStreamDataPtr>* observer) override;
	void unsubscribe (devices::IDeviceObserver<IStreamDataPtr>* observer) override;
	bool has_observer(devices::IDeviceObserver<IStreamDataPtr>* observer) override;

	void unsubscribe_all() override;
	size_t count() const override;

	private:
		DirectshowDeviceListener(const DirectshowDeviceListener& other) = delete;
		DirectshowDeviceListener& operator=(const DirectshowDeviceListener&) = delete;

		devices::IDeviceInfo<DirectShowDeviceInfo>* device_connectivity_;
	  std::unique_ptr<VideoSource> video_capture_;
		std::mutex mutex_;

		std::string device_name_;
		std::queue<video::PlayerCommands> commands_;
				
		std::chrono::milliseconds delay_between_commands_
			= std::chrono::milliseconds(0);

		std::chrono::milliseconds empty_command_timeout_
			= std::chrono::milliseconds(200);
	};

	typedef std::shared_ptr<DirectshowDeviceListener> DirectshowDeviceListenerPtr;

}

#endif
