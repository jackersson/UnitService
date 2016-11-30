#include "access_device/commands/command.hpp"

#include <access_device/rs232/rs232_controller_utils.hpp>
#include <vector>
#include <sstream>

namespace access_device
{
	namespace commands {
			
		OutputCommand::OutputCommand(const std::vector<unsigned char>& command)
		{
			valid_ = false;
			parse(command);
		}

		bool OutputCommand::is_valid() const
		{
			return valid_;
		}

		bool OutputCommand::is_empty() const 
		{
			return small_data() == 0 || rs232::data_empty(data_);
		}

		uint16_t OutputCommand::device_number() const {
			return device_number_;
		}

		unsigned char OutputCommand::small_data() const
		{
			if (data_.size() <= 0)
				return 0;
			return data_[0];
		}

		std::string OutputCommand::get_dallas_key() const
		{
			std::stringstream ss;
			for (auto bt : data_)
				ss << static_cast<int>(bt);
			return ss.str();
		}

		const std::vector<unsigned char>& OutputCommand::full_data() const {
			return data_;
		}

		access_device_module OutputCommand::device_module() const
		{
			switch (header_)
			{
			case rs232::WriteB: return Lights;
			case rs232::ReadC : return Buttons;
			case rs232::Dallas: return Dallas;
			case rs232::ReadB : return Info;
			case rs232::Master:
			default:
				break;
			}
			return NoneModule;
		}

	
		void OutputCommand::parse(const std::vector<unsigned char>& command)
		{
			if (command.size() < rs232::READ_COMMAND_SIZE)
				return;

			auto pos = 0;

			int check_sum_offset = command.size() - rs232::PAIR;
			unsigned char sum = 0;
			unsigned char check_sum = 0;
			unsigned char previous_item = 0;
			for (const auto& item : command)
			{
				if (pos == rs232::HEADER_INDEX)
					header_ = static_cast<rs232::port_command>(item);
				else if (pos == (rs232::DEVICE_NUMBER_OFFSET + 1))
					device_number_ = rs232::byte_pair_to_uint(previous_item, item);
				else if (pos >= rs232::DATA_OFFSET + 1
					&& pos < check_sum_offset
					&& pos % 2 == 0)
					data_.push_back(rs232::word_pair_to_byte(previous_item, item));
				else if (pos > check_sum_offset)
					check_sum = rs232::word_pair_to_byte(previous_item, item);

				previous_item = item;
				if (pos < check_sum_offset)
					sum += item;
				pos++;
			}

			valid_ = rs232::check_sum_valid(sum, check_sum);

			if (header_ == rs232::ReadC || header_ == rs232::Dallas)
				rs232::invert(data_);
		}		

		//*** Simple command	
		SimpleCommand::SimpleCommand(rs232::port_command  port_cmd
			, size_t size)
		{
			if (size < rs232::READ_COMMAND_SIZE) 
				throw std::exception("Command size cannt be less than 5");
			command_.resize(size);
			set_header(port_cmd);
			SimpleCommand::set_device_number(0);
		}

		rs232::port_command SimpleCommand::get_header() const  {
			return header_;
		}

		void SimpleCommand::set_header(rs232::port_command  port_cmd)
		{
			if (header_ == port_cmd)
				return;
			header_ = port_cmd;
			rs232::set_header(command_, header_);
		}

		void SimpleCommand::set_device_number(uint16_t dev_number) 
		{
			if (device_number_ == dev_number)
				return;
			device_number_ = dev_number;
			rs232::set_device_number(command_, device_number_);
			rs232::set_check_sum(command_);
		}

		const std::vector<unsigned char>& SimpleCommand::get() const  {
			return command_;
		}
	
		// ***** Read command ***** 	
		ReadPortCommand::ReadPortCommand(rs232::port_command port_cmd)
			: SimpleCommand(port_cmd, rs232::READ_COMMAND_SIZE)
		{
			//rs232::set_check_sum(command_);
		}

		void ReadPortCommand::set_data(unsigned char data)  {
			//Not implemented
		}
	
		// ****** Write command ****** 
		WritePortCommand::WritePortCommand(rs232::port_command port_cmd)
			: SimpleCommand(port_cmd, rs232::DATA_SIZE)
		{
			WritePortCommand::set_data(0);
		}

		void WritePortCommand::set_data(unsigned char data)
		{
			if (data == data_)
				return;
			rs232::set_data(command_, data);
			rs232::set_check_sum(command_);
		}

 //****** Dallas command ******* 

		DallasKeyCommand::DallasKeyCommand(rs232::port_command port_cmd)
			: SimpleCommand(port_cmd, rs232::DALLAS_SIZE)
		{
		}

		void DallasKeyCommand::set_data(unsigned char)
		{
			//Not implemented
		}

		void DallasKeyCommand::set_data(const std::vector<unsigned char>& data)
		{
			if (data == data_)
				return;
			rs232::set_data_key(command_, data_, rs232::DATA_OFFSET);
			rs232::set_check_sum(command_);
		}
	}
}
