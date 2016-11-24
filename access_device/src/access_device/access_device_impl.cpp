#include "access_device/access_device_listener.hpp"


#include <access_device/core/timeout_serial.hpp>
#include <access_device/access_device_impl.hpp>
#include <future>

using namespace data_model;
using namespace contracts::devices::access_device;
using namespace std::chrono;

namespace access_device
{	
	AccessDeviceImpl::AccessDeviceImpl(const std::string& port_name)
		: port_name_(port_name)
		, serial_port_(std::make_unique<TimeoutSerial>())
	{
		init();
	}

	void AccessDeviceImpl::init()
	{
			std::async(std::launch::async, [this]() {
		try
		{
			auto state = open();

			if (state)
			{
				auto serial_number = factory_.get_device_number(*serial_port_);
				if (serial_number != commands::CommandFactory::DEVICE_ERROR)
					device_number_ = std::pair<uint16_t, bool>(serial_number, true);
				std::cout << port_name_ << " "
					<< device_number_.first << " " << device_number_.second <<  " "
					<< serial_number << std::endl;
			}
			else
				std::cout << port_name_ << " not controlller" << std::endl;
			close();
		}
		catch (std::exception& ex)
		{
			std::cout << port_name_ << " init exception " << ex.what() << std::endl;
		}
			});
	}

	AccessDeviceImpl::~AccessDeviceImpl() {
		de_init();
	}

	void AccessDeviceImpl::de_init()
	{		
		reset();
		close();
	}

	bool AccessDeviceImpl::reset()
	{
		if (serial_port_ == nullptr)
			return false;
		return factory_.reset(*serial_port_); //All lights should be reset
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
		if (!serial_port_->is_open())
			return;
		try
		{
			serial_port_->close();
		}
		catch (std::exception&) {
			//Not implemented
		}
	}	
}


