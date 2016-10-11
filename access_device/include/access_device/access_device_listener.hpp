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
			//serial_port_.set_option(boost::asio::serial_port_base::baud_rate(BAUD_RATE));
		}

		void stop() override
		{
			clear();
			execute<commands::LightCommandImpl>();
			wait_reset();
			Threadable::stop();
		}

		//TODO make private
		void wait_reset()
		{
			std::lock_guard<std::mutex> lock(mutex_);
			auto start_ticks = clock();
			while (commands_.empty() && (clock() - start_ticks) < read_write_timeout.count())
				std::this_thread::sleep_for(delay_between_ask_device_);

		}

	protected:
		void run() override
		{
			open();

			while (active())
			{
				auto command = dequeue();
				if (command != nullptr)
				{
					std::this_thread::sleep_for(delay_between_ask_device_);

					auto execution_result = command->execute(serial_port_);
					if (execution_result->ok())
					{
						if (!execution_result->empty())
							on_next(execution_result);

						if (need_to_recover_)
							unlock();
					}
					else
						handle_exception(execution_result->exception());

				}
			}

			serial_port_.close();
			clear();
		}

		void handle_exception(const std::exception& ex)
		{
			if (!is_active())
			{
				on_error(ex);
				std::this_thread::sleep_for(delay_between_ask_device_);
				open();
			}
			else if (ex.what() != "Undefined exception")
			{
				on_error(ex);
				lock();
			}
			else
			{
				on_state(DataTypes::DeviceState::Active);
				unlock();
			}
		}

		void lock()
		{
			need_to_recover_ = true;
		}

		void unlock()
		{
			if (!need_to_recover_)
				return;
			execute<commands::LightCommandImpl>(contracts::devices::access_device::lights::lRedMain);
			need_to_recover_ = false;
		}
	public:
		void execute(core::IExecutableCommandPtr command)
		{
			std::lock_guard<std::mutex> lock(mutex_);
			commands_.push(command);
		}

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
			std::lock_guard<std::mutex> lock(mutex_);
			while (!commands_.empty())			
				commands_.pop();						
		}

	  static	const int BAUD_RATE = 4800;

	private:

	  void open()
	  {
			if (serial_port_.is_open())
				return;

			try
			{
				serial_port_.open(device_name_, BAUD_RATE);		
				serial_port_.set_timeout(boost::posix_time::millisec(100));
				if (!factory_.reset(serial_port_))
					throw std::exception("Can't reset device");

				execute<commands::LightCommandImpl>(contracts::devices::access_device::lights::lRedMain);

				on_state(DataTypes::DeviceState::Active);

				need_to_recover_ = false;
			}
			catch (std::exception& exception)	{
				//TODO log
				on_error(exception);
			}
	  }

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


		core::IExecutableCommandPtr dequeue()
		{
			std::lock_guard<std::mutex> lock(mutex_);
			while (commands_.empty())
			{
				std::this_thread::sleep_for(delay_between_ask_device_);
				what_with_access_device();
			}

			return commands_.front();			
		}

		void what_with_access_device()
		{
			if (need_to_ask_buttons_)
				execute<commands::ButtonCommandImpl>();
			else
				execute<commands::DallasCommandImpl>();
			need_to_ask_buttons_ = !need_to_ask_buttons_;
		}
		
		AccessDeviceListener(const AccessDeviceListener& other) = delete;
		AccessDeviceListener& operator=(const AccessDeviceListener&) = delete;

		commands::CommandFactory factory_;

		std::mutex mutex_;

		std::string device_name_;
		std::queue<core::IExecutableCommandPtr> commands_;

	//	boost::asio::io_service  io_service_;
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
