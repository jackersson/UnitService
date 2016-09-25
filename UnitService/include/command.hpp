#ifndef Command_Included
#define Command_Included

#include "iequatable.hpp"
#include "rs232_controller_utils.hpp"

#include <vector>

namespace utils
{
	class Command : public IEquatable<Command>
	{
	public:

		explicit Command(const std::vector<unsigned char>& bytes)
			: bytes_(bytes)
		{
			valid_ = rs323_controller::check_sum_valid(bytes);

			data_  = rs323_controller::get_data(bytes);
			empty_ = rs323_controller::data_empty(data_);
		}

		virtual ~Command() {}

		bool equals(const Command& other) const override
		{
			return valid() && other.valid() 
				             && rs323_controller::compare(bytes(), other.bytes());
		}

		bool valid() const		{
			return valid_;
		}

		bool empty() const {
			return empty_;
		}

		const std::vector<unsigned char>& bytes() const 	{
			return bytes_;
		}

		const std::vector<unsigned char>& data() const		{
			return data_;
		}

	protected:
		std::vector<unsigned char> data_;
		bool empty_;

	private:
		std::vector<unsigned char> bytes_;
		bool valid_;
	};

	//if 0 then 255 in data
	class InvertedCommand : public Command
	{
		explicit InvertedCommand(const std::vector<unsigned char>& bytes)
			: Command(bytes)
		{
			if (data_.size() == 0)
				return;
			rs323_controller::invert(data_);
			empty_ = rs323_controller::data_empty(data_);
		}
	};
}

#endif