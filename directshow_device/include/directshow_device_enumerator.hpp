#ifndef DirectShowDeviceEnumerator_Included
#define DirectShowDeviceEnumerator_Included

#include <threadable.hpp>
#include <idevice_enumerator.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include <algorithm>

namespace directshow_device
{
	class directshow_device_enumerator : public utils::threadable
		, public contracts::devices::idevice_enumerator
	{
	public:
		bool connected(const std::string& device_name) const override {
			return contains<std::string>(devices_, device_name);
		}

		std::vector<std::string> devices() const override {
			return devices_;
		}

	protected:
		void run() override
		{
			while (active())
			{
				enumerate();
				update();
				print();
				std::this_thread::sleep_for(delay_);
			}
		}

		void enumerate()
		{
			actual_.clear();
			auto devices_count = 1;
			for (auto i = 0; i < devices_count; ++i)
			{
				actual_.push_back("test");
			}
		}

		void print()
		{
			std::cout << std::endl;
			for (auto device_name : actual_)
				std::cout << device_name << std::endl;
			std::cout << std::endl;
		}

	private:

		void update()
		{
			if (devices_.size() > 0)
			{

				auto predicate = [this](const std::string& device_name) {
					return contains(actual_, device_name);
				};

				devices_.erase(std::remove_if(devices_.begin(), devices_.end(), predicate)
					, devices_.end());
			}

			for (auto device_name : actual_)
			{
				if (!contains(devices_, device_name))
					devices_.push_back(device_name);
			}
		}


		//TODO move to utils
		template<typename T>
		bool contains(const std::vector<T>& container, T value) const
		{
			for (auto it : container)
			{
				if (it == value)
					return true;
			}
			return false;
		}


		std::vector<std::string> devices_;
		std::vector<std::string> actual_;

		std::chrono::milliseconds delay_ = std::chrono::milliseconds(1000);

	};
}

#endif
