#ifndef AccessDeviceImpl_Included
#define AccessDeviceImpl_Included

#include <access_device/core/iexecutable_command.hpp>
#include "commands/command_factory.hpp"
#include "logging/logger.hpp"

namespace data_model {
	class DeviceId;
}

namespace access_device
{
	namespace commands {
		class CommandFactory;
	}
	class AccessDeviceImpl final 
	{
	public:
		explicit AccessDeviceImpl(const std::string& port_name);
		
		~AccessDeviceImpl();

		//Throws exception
		ICommandResultPtr	execute(core::IExecutableCommandPtr command);

		void init   ();
		void de_init();
		bool open   ();
		bool reset  ();
			
		bool is_open() const;

		commands::CommandFactory& factory()	{
			return factory_;
		}

		const std::string& port_name() const {
			return port_name_;
		}

		uint16_t id() const {
			return device_number_.first;
		}

		bool device_valid() const {
			return device_number_.second;
		}

		static const int BAUD_RATE = 4800;

	private:
		AccessDeviceImpl(const AccessDeviceImpl& other) = delete;
		AccessDeviceImpl& operator=(const AccessDeviceImpl&) = delete;
		
		void close();

		void try_init();	

		bool busy_;

		commands::CommandFactory factory_;
		std::recursive_mutex     mutex_;

		std::string               port_name_;
		std::pair<uint16_t, bool> device_number_;

		contracts::logging::Logger logger_;

		std::unique_ptr<TimeoutSerial>   serial_port_;	
	};

	typedef std::shared_ptr<AccessDeviceImpl> AccessDeviceImplPtr;

}

#endif
