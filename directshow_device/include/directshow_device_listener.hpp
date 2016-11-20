#ifndef DirectShowDeviceListener_Included
#define DirectShowDeviceListener_Included

#include <queue>
#include <threadable.hpp>
#include <mutex>
#include <contracts/video/iplayer.hpp>
#include <contracts/devices/idevice_info.hpp>
#include <observers/observable.hpp>
#include <contracts/devices/video_device/istream_data.hpp>
#include <contracts/devices/device_observer.hpp>

namespace directshow_device
{

	typedef
		contracts::devices::IDeviceObserver
		<contracts::devices::video_device::IStreamData>	IVideoDeviceObserver;


	class VideoSource;
	class Capability;
	class DirectShowDeviceInfo;

	class DirectshowDeviceListener  
		     : public utils::Threadable
		     , public contracts::video::IMediaPlayer
		     , public contracts::observers::IObservable<IVideoDeviceObserver>
	{
	public:
		explicit 
			DirectshowDeviceListener
			( const std::string& device_name
			, contracts::devices::IDeviceInfo<DirectShowDeviceInfo>* device_holder);
		
		~DirectshowDeviceListener();

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

		void open(DirectShowDeviceInfo& device);

		bool is_active() const;

	protected:
		void run  () override;
		void clear();

		contracts::video::PlayerCommands dequeue();

public:
	void subscribe   (IVideoDeviceObserver* observer) override;
	void unsubscribe (IVideoDeviceObserver* observer) override;
	bool has_observer(IVideoDeviceObserver* observer) override;

	void unsubscribe_all() override;
	size_t count() const override;

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
