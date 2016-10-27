#ifndef AccessDeviceListener_Included
#define AccessDeviceListener_Included

#include <queue>

#include <threadable.hpp>
#include <access_device/core/iexecutable_command.hpp>
#include <mutex>
#include <contracts/devices/device_observer.hpp>
#include <contracts/observers/observable..hpp>
#include "commands/command_factory.hpp"
#include <datatypes/devices.pb.h>
#include "common/access_device_state.hpp"

namespace access_device
{	
	typedef contracts::devices::access_device::ICommandResult ICommandResult;
	typedef contracts::devices::IDeviceObserver<ICommandResult> IAccessDeviceObserver;
	typedef std::shared_ptr<IAccessDeviceObserver> IAccessDeviceObserverPtr;
	typedef std::pair<core::ICommandContextPtr, unsigned char> ExecutableDataCommand;

	class AccessDeviceListener final : public utils::Threadable
		                   , public contracts::observers::Observable<IAccessDeviceObserver>
		                   
	{
	public:
		explicit AccessDeviceListener(std::string device_name)
			: factory_(0)
			, device_name_(device_name)
			, serial_port_()
			, need_to_ask_buttons_(false)
			, need_to_recover_(false)
		{
			
		}
		~AccessDeviceListener()	{
			AccessDeviceListener::stop();
		}

		void stop() override;

	protected:
		void run() override;

		void handle_exception(const std::exception& ex);

		void lock();

		void unlock();
	public:
		void execute(core::IExecutableCommandPtr command);

		template <typename T>
		void execute(unsigned char data = 0)
		{
			auto command = factory_.get<T>(data);		
			execute(command);
		}

		bool is_active() const
		{
			return serial_port_.is_open() && active();
		}

		void clear()
		{
			std::lock_guard<std::recursive_mutex> lock(mutex_);
			while (!commands_.empty())			
				commands_.pop();						
		}

		static	const int BAUD_RATE = 4800;
	private:

		void open(); 

		void 
		on_error(const std::exception& exception)  
	  {
		  contracts::devices::DeviceException 
				device_exception(exception.what(), DataTypes::DeviceType::CardReader);

			for (auto observer : observers_)
				observer->on_error(device_exception);
	  }

		void 
		on_state(DataTypes::DeviceState state) 
	  {
			common::AccessDeviceState ac_state(state);
			for (auto observer : observers_)
				observer->on_state(ac_state);
	  }

		void 
		on_next(contracts::devices::access_device::ICommandResultPtr data)
	  {
			for (auto observer : observers_)
				observer->on_next(*data.get());
	  }

		core::IExecutableCommandPtr dequeue();

		void what_with_access_device();
		
		AccessDeviceListener(const AccessDeviceListener& other) = delete;
		AccessDeviceListener& operator=(const AccessDeviceListener&) = delete;

		commands::CommandFactory factory_;

		std::recursive_mutex mutex_;

		std::string device_name_;
		std::queue<core::IExecutableCommandPtr> commands_;

		TimeoutSerial serial_port_;

		bool need_to_ask_buttons_;
		bool need_to_recover_;
		
		std::chrono::milliseconds read_write_timeout
			= std::chrono::milliseconds(1000);

		std::chrono::milliseconds delay_between_ask_device_
			                                  = std::chrono::milliseconds(100);

	};

	typedef std::shared_ptr<AccessDeviceListener> AccessDeviceListenerPtr;
	
}

#endif
