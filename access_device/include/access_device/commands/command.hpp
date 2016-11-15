#ifndef Command_Included
#define Command_Included

#include <vector>
#include <contracts/devices/access_device/access_device_types.hpp>
#include <contracts/devices/access_device/icommand_result.hpp>
#include <access_device/rs232/rs232_controller_types.hpp>

namespace access_device
{
	namespace commands 
	{
		class OutputCommand 
			: public contracts::devices::access_device::ICommandResult
		{
		public:
			explicit OutputCommand(const std::vector<unsigned char>& command);

			bool is_valid() const override;
			bool is_empty() const override;

			uint16_t      device_number () const override;
			std::string   get_dallas_key() const override;
			unsigned char small_data    () const override;

			const std::vector<unsigned char>& full_data() const override;

			contracts::devices::access_device::access_device_module
				device_module() const override;

		private:
			void parse(const std::vector<unsigned char>& command);

			rs232::port_command        header_       ;
			uint16_t                   device_number_;
			std::vector<unsigned char> data_         ;
			bool                       valid_        ;
		};

		class IInputCommand
		{
		public:
			virtual ~IInputCommand() {}

			virtual void set_device_number(uint16_t      dev_number) = 0;
			virtual void set_data         (unsigned char data      ) = 0;

			virtual const std::vector<unsigned char>& get() const = 0;

			virtual rs232::port_command get_header() const = 0;
		};

		class SimpleCommand : public IInputCommand
		{
		public:
			SimpleCommand(rs232::port_command port_cmd, size_t size);

			rs232::port_command get_header() const override;

			void set_header       (rs232::port_command port_cmd);
			void set_device_number(uint16_t dev_number) override;

			const std::vector<unsigned char>& get() const  override;
			
		protected:
			std::vector<unsigned char> command_;

		private:
			rs232::port_command  header_;
			uint16_t      device_number_;
		};

		class ReadPortCommand : public SimpleCommand
		{
		public:
			explicit ReadPortCommand(rs232::port_command port_cmd);

			void set_data(unsigned char data) override;
		};

		class WritePortCommand : public SimpleCommand
		{
		public:
			explicit WritePortCommand(rs232::port_command port_cmd);

			void set_data(unsigned char data) override;
		private:
			unsigned char data_;
		};

		class DallasKeyCommand : public SimpleCommand
		{
		public:
			explicit DallasKeyCommand(rs232::port_command port_cmd);

			void set_data(unsigned char) override;

			void set_data(const std::vector<unsigned char>& data);
		private:
			std::vector<unsigned char> data_;
		};
	}
}

#endif