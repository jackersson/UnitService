/*#ifndef UnitServiceApi_Included
#define UnitServiceApi_Included

#include <future>

namespace testable_unit_context
{
	inline
	bool check_device( const contracts::devices::IDeviceEnumerator& enumerator
		               , const data_model::DeviceId& device) 
	{
		auto tries_count = 0;
		auto max_tries_count = 10;
		while (tries_count < max_tries_count)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			auto flag = enumerator.connected(device);
			if (flag)
				return true;
			tries_count++;
		}
		return false;
	}

	class GetCardApi
		: public contracts::devices::IDeviceObserver
		<contracts::devices::access_device::ICommandResult>
	{
	public:
		explicit GetCardApi
		( contracts::devices::access_device::IAccessDeviceEngine* engine)
			: engine_(engine)
		{
			if (engine_ == nullptr)
				throw std::exception("AccessDeviceEngine can't be null");
		}

		//TODO timeout
		std::string	get_card( const data_model::DeviceId& device
		                    , std::chrono::milliseconds timeout = std::chrono::milliseconds(5000))
		{
			std::string card_number = "";
			if (!check_device(engine_->enumerator(), device))
				return card_number;

			engine_->add(device);
			engine_->subscribe(this, device);
			
			try	{
				card_number = utils::service::get_result(card_number_, timeout);
			}
			catch (std::exception& ex){
				//Not implemented
			}

			engine_->remove(device);
			engine_->unsubscribe(this);

			return card_number;
		};

		void on_error(const contracts::devices::DeviceException& exception) override
		{}

		void on_state(const contracts::devices::IDeviceState&    state) override 
		{}

		void on_next(const contracts::devices::access_device::ICommandResult& data) override
		{
			if (data.device_module() == contracts::devices::access_device::Dallas)
				card_number_.set_value(data.get_dallas_key());
		}

	private:
		GetCardApi(const GetCardApi& other) = delete;
		GetCardApi& operator=(const GetCardApi&) = delete;

		contracts::devices::access_device::IAccessDeviceEngine* engine_;
	  std::promise<std::string> card_number_;
	};
	
}

#endif */

