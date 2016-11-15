#ifndef AccessDeviceListener_Included
#define AccessDeviceListener_Included

#include <access_device/core/iexecutable_command.hpp>
#include <contracts/devices/device_observer.hpp>
#include <observers/observable.hpp>
#include <contracts/devices/idevice_info.hpp>
#include "access_device_impl.hpp"
#include <threadable.hpp>
#include <queue>

namespace data_model{
	class DeviceId;
}

namespace access_device
{	
	namespace commands {
		class CommandFactory;
	}

	typedef contracts::devices::access_device::ICommandResult ICommandResult;
	typedef contracts::devices::IDeviceObserver<ICommandResult> IAccessDeviceObserver;
	typedef std::shared_ptr<IAccessDeviceObserver> IAccessDeviceObserverPtr;

	class AccessDeviceListener final : public utils::Threadable
		                               , public contracts::observers::Observable<IAccessDeviceObserver>
		                   
	{
	public:

		explicit 
			AccessDeviceListener(const data_model::DeviceId& port_name
			     , contracts::devices::IDeviceInfo<AccessDeviceImplPtr>* devices);
		
		~AccessDeviceListener();

		void stop() override;

		void execute(core::IExecutableCommandPtr command);

		template <typename T>
		void execute(unsigned char data = 0)
		{
			std::lock_guard<std::recursive_mutex> lock(mutex_);
			//TODO think about smarter way
			if (access_device_impl_ == nullptr)
				handle_exception(std::exception("Device is null"));
			if (access_device_impl_ == nullptr)
				return;
			auto command = access_device_impl_->factory().get<T>(id(), data);		
			execute(command);
		}

		bool is_active() const;	

		void clear()
		{
			std::lock_guard<std::recursive_mutex> lock(mutex_);
			while (!commands_.empty())			
				commands_.pop();						
		}

		const std::string& port_name() const;

		uint16_t id() const;
		
		static const int BAUD_RATE = 4800;

	protected:
		void run() override;

	private:
		AccessDeviceListener(const AccessDeviceListener& other) = delete;
		AccessDeviceListener& operator=(const AccessDeviceListener&) = delete;
		
		void handle_exception(const std::exception& ex);

		void lock  ();
		void unlock();
		bool open  (); 
		void activate();
		//void close ();
		//void reset ();
		void init_session();

		void on_error(const std::exception& exception);
		void on_state(data_model::DeviceState state);
		void on_next (contracts::devices::access_device::ICommandResultPtr data);

		core::IExecutableCommandPtr dequeue();

		void what_with_access_device();
		

		//commands::CommandFactory factory_;
		std::recursive_mutex     mutex_  ;

		//std::string               port_name_    ;
		std::unique_ptr<data_model::DeviceId> device_number_;

		//std::unique_ptr<data_model::DeviceId>   device_info_;
		std::queue<core::IExecutableCommandPtr> commands_   ;
		//std::unique_ptr<TimeoutSerial>          serial_port_;

		AccessDeviceImplPtr access_device_impl_;

		contracts::devices::IDeviceInfo<AccessDeviceImplPtr>* devices_;

		bool need_to_ask_buttons_;
		bool need_to_recover_    ;


		
		static std::chrono::milliseconds read_write_timeout_      ;
		static std::chrono::milliseconds delay_between_ask_device_;
	};

	typedef std::shared_ptr<AccessDeviceListener> AccessDeviceListenerPtr;	
}

#endif
