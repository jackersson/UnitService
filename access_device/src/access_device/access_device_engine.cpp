#include <access_device/access_device_engine.hpp>
#include <future>
#include <service_utils.hpp>

using namespace contracts::devices;
using namespace contracts::devices::access_device;
using namespace std::chrono;

struct AccessDeviceCommandResult
{
	std::string card_number;
	std::string error_message;
	
};

typedef std::shared_ptr<AccessDeviceCommandResult> AccessDeviceCommandResultPtr;

struct AsyncAccessDeviceContext
{
	std::promise<AccessDeviceCommandResultPtr> promise;
};


class TestableAccessDeviceObserver : public IDeviceObserver<ICommandResult>
{
public:
	explicit TestableAccessDeviceObserver(AsyncAccessDeviceContext* context)
		: context_(context)
	{}

	void on_error(const contracts::devices::DeviceException& exception) override
	{
		auto object = std::make_shared<AccessDeviceCommandResult>();
		object->error_message = exception.what();
		context_->promise.set_value(object);
	}

	void on_state(const IDeviceState& state) override {	}

	void on_next(const ICommandResult& data) override
	{		
		if (data.module()	== Dallas)
		{
			auto object = std::make_shared<AccessDeviceCommandResult>();
			object->card_number = data.to_string();
			context_->promise.set_value(object);
		}
	}

private:
	AsyncAccessDeviceContext* context_;
};

bool 
do_check_access_device( IAccessDeviceEngine* engine
	                    , const std::string& device_name
	                    , milliseconds timeout = milliseconds(100));

namespace access_device
{
	bool AccessDeviceEngine::grant_access(const std::string& device_name)
	{
		return do_check_access_device(this, device_name, milliseconds(500));
	}

	bool AccessDeviceEngine::check(const std::string& device_name)
	{
		return do_check_access_device(this, device_name);
	}

	std::string AccessDeviceEngine::get_card(const std::string& device_name)
	{
		AsyncAccessDeviceContext context;
		TestableAccessDeviceObserver observer(&context);

		add      (device_name);
		subscribe(&observer, device_name);
		std::this_thread::sleep_for(std::chrono::seconds(1));

		AccessDeviceCommandResultPtr result = nullptr;
		try
		{
			result = utils::service::get_result(context.promise
				, std::chrono::milliseconds(500));

		}
		catch (std::exception& exception)	{
		  throw exception;
		}

		unsubscribe(&observer);
		remove(device_name);
				
		if (result != nullptr)
		{
			if (result->error_message != "" )
				throw std::exception(result->error_message.c_str());
			if (result->card_number == "")
				throw std::exception("Card is empty");
		}
		else 
			throw std::exception("Null reference exception");

		return result->card_number;
	}
}

bool
do_check_access_device( IAccessDeviceEngine* engine
	                    , const std::string& device_name
                      , milliseconds timeout )
{
	if (engine == nullptr)
		return false;

	AsyncAccessDeviceContext context;
	TestableAccessDeviceObserver observer(&context);

	engine->add(device_name);
	engine->subscribe(&observer, device_name);
	std::this_thread::sleep_for(std::chrono::seconds(1));


	AccessDeviceCommandResultPtr result = nullptr;
	try
	{
		engine->execute(device_name, lGreenAccess);
		result = utils::service::get_result(context.promise, timeout);
	}
	catch (std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}

	engine->unsubscribe(&observer);
	engine->remove(device_name);

	if (result != nullptr)
	{
		if (result->error_message != "")
			throw std::exception(result->error_message.c_str());
	}

	return true;
}




