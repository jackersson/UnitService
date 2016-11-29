#include "access_device/access_device_listener.hpp"

#include "access_device/core/timeout_serial.hpp"
#include "access_device/access_device_impl.hpp"

#include <thread_pool.hpp>

using namespace data_model;
using namespace std::chrono;

namespace access_device
{	
	AccessDeviceImpl::AccessDeviceImpl(const std::string& port_name)
		: busy_(false)
		, port_name_(port_name)	
		, serial_port_(std::make_unique<TimeoutSerial>())
	{
		open();
		init();
	}

	void AccessDeviceImpl::init()
	{
		if (busy_ || device_valid())
			return;

		busy_ = true;		

		utils::threading::submit_job_nowait(
			[this]() 
		{
			try_init();
			std::this_thread::sleep_for(seconds(2));
			busy_ = false;
		});
	}

	void AccessDeviceImpl::try_init()
	{
		try
		{
			if (!is_open())
				return;

			auto serial_number = factory_.get_device_number(*serial_port_);
			if (serial_number != commands::CommandFactory::DEVICE_ERROR)
			{
				device_number_ = std::pair<uint16_t, bool>(serial_number, true);
				logger_.info("{0} : is controller with device id {1}", port_name_, device_number_.first);
			}
		}
		catch (std::exception&) {}
	}	

	AccessDeviceImpl::~AccessDeviceImpl() {
		de_init();
	}

	void AccessDeviceImpl::de_init()
	{		
		if (!is_open())
			return;
		reset();
		close();
	}

	bool AccessDeviceImpl::reset()
	{
		if (serial_port_ == nullptr || !device_valid())
			return false;
		auto ok = factory_.reset(*serial_port_);
		logger_.info("{0} : reset {1}", port_name_, ok ? "succeed" : "failed");
		return ok; //All lights should be reset
	}
	
	bool AccessDeviceImpl::is_open() const
	{	
		return serial_port_->is_open();
	}

	
	ICommandResultPtr
		AccessDeviceImpl::execute(core::IExecutableCommandPtr command)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		return command->execute(*serial_port_);
	}

	bool AccessDeviceImpl::open()
	{
		if (serial_port_->is_open())
			return true;
		
		serial_port_->open(port_name_, BAUD_RATE);
		serial_port_->set_timeout(boost::posix_time::millisec(1000));
		return true;	
	}

	void AccessDeviceImpl::close()
	{	
		try
		{
			serial_port_->close();
		}
		catch (std::exception&) {
			//Not implemented
		}
	}	
}


