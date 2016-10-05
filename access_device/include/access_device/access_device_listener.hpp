#ifndef AccessDeviceListener_Included
#define AccessDeviceListener_Included

#include <queue>

#include <threadable.hpp>
#include <access_device/core/iexecutable_command.hpp>
#include <mutex>
#include <contracts/devices/device_observer.hpp>
#include <contracts/observers/observable..hpp>

namespace access_device
{
	typedef contracts::devices::access_device::ICommandResult ICommandResult;
	typedef contracts::devices::IDeviceObserver<ICommandResult> IAccessDeviceObserver;

	class AccessDeviceListener final : public utils::Threadable
		                   , public contracts::observers::Observable<IAccessDeviceObserver>
		                   , IAccessDeviceObserver
	{
	public:
		explicit AccessDeviceListener(std::string device_name)
			: device_name_(device_name)
			, serial_port_(io_service_)
		{}

	protected:
		void run() override
		{
			open();

			while (active())
			{
			
			}
		}

		void execute(core::IExecutableCommandPtr command)
		{
			std::lock_guard<std::mutex> lock(mutex_);
			commands_.push(command);
		}

		template <typename T>
		void execute(unsigned char command)
		{

		}

		bool is_active() const
		{
			return serial_port_.is_open() && active();
		}

		void clear()
		{
			std::lock_guard<std::mutex> lock(mutex_);
			while (!commands_.empty())			
				commands_.pop();						
		}

	private:

	  void open()
	  {
			if (serial_port_.is_open())
				return;

			try
			{
				serial_port_.open(device_name_);				
			}
			catch (std::exception&)
			{
				//Not Implemented
			}
	  }

		void 
		on_error(const contracts::devices::DeviceException& exception) const override
	  {
		  
	  }

		void 
		on_state(const contracts::devices::IDeviceState& state) const override
	  {
		  
	  }

		void 
		on_next(const ICommandResult& data) const override
	  {
		  
	  }

		int get_hash_code() const override  {
			return 0;
	  }


		/*
		const IExecutableCommand& dequeue()
		{
			std::lock_guard<std::mutex> lock(mutex_);
				while (commands_.empty())
				{
					std::this_thread::sleep_for(delay_between_ask_device_);
					AskCommand();
				}

				return _commands.TryDequeue(out command);
			
		}

		void what_with_access_device()
		{
			if (_flag)
				execute()<ButtonCommandImpl>();
			else
				execute<DallasCommandImpl>();
			_flag = !_flag;
		}
		*/
		AccessDeviceListener(const AccessDeviceListener& other) = delete;
		AccessDeviceListener& operator=(const AccessDeviceListener&) = delete;


		std::mutex mutex_;

		std::string device_name_;
		std::queue<core::IExecutableCommandPtr> commands_;

		boost::asio::io_service  io_service_;
		boost::asio::serial_port serial_port_;

		std::chrono::milliseconds delay_between_ask_device_
			                                  = std::chrono::milliseconds(100);

	};

	typedef std::shared_ptr<AccessDeviceListener> AccessDeviceListenerPtr;

}

#endif
