#ifndef Command_Included
#define Command_Included

#include <access_device/rs232/rs232_controller_utils.hpp>
#include <vector>
#include <contracts/common/iequatable.hpp>

namespace access_device
{
	namespace commands {

		class Command : public contracts::common::IEquatable<Command>
		{
		public:

			explicit Command(const std::vector<unsigned char>& bytes)
				: bytes_(bytes)
			{
				valid_ = rs232::check_sum_valid(bytes);

				data_  = rs232::get_data(bytes);
				empty_ = rs232::data_empty(data_);
			}

			virtual ~Command() {}

			bool equals(const Command& other) const override
			{
				return valid() && other.valid()
					&& rs232::compare(bytes(), other.bytes());
			}

			bool valid() const {
				return valid_;
			}

			bool empty() const {
				return empty_;
			}

			const std::vector<unsigned char>& bytes() const {
				return bytes_;
			}

			const std::vector<unsigned char>& data() const {
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
			public:
			explicit InvertedCommand(const std::vector<unsigned char>& bytes)
				: Command(bytes)
			{
				if (data_.size() == 0)
					return;
				rs232::invert(data_);
				empty_ = rs232::data_empty(data_);
			}
		};
	}
}

#endif