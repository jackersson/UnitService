#ifndef GetCardMock_Included
#define GetCardMock_Included

#include <data/models/devices.hpp>
#include <gtest/gtest.h>
#include <boost/thread/future.hpp>

namespace testable_unit_context
{	
	class GetCardMock 
	{
	public:
		explicit GetCardMock(contracts::devices::access_device::IAccessDeviceEngine* engine)
			: done_(false), card_api_(engine)
		{}
		
		void process_request(const DataTypes::Device& request)
		{
			request_ = request;

			if (request_.device_type() == DataTypes::DeviceType::CardReader)
			{
				auto serial = static_cast<uint16_t>(request_.serial_number());
				data_model::DeviceId dev("any", serial);
							
				boost::async([this, dev]() {
					return card_api_.get_card(dev);
				})
				.then([this](boost::future<std::string> f) {
					prepare_response(f.get());
				}).get();
			}
			else
				complete(DataTypes::CardMsg());
		}

		void prepare_response(const std::string& card_number)
		{
			DataTypes::CardMsg response;
			response.set_card_num(card_number);		
			complete(response);
		}	 

	private:
	  void complete(const DataTypes::CardMsg& response)
		{
			EXPECT_NE  ("", response.card_num());
		}
		
		bool done_;
		DataTypes::Device  request_ ;
		GetCardApi         card_api_;

		const std::chrono::seconds REQUEST_TIMEOUT = std::chrono::seconds(5);
	};
	
}
#endif