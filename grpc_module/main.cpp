#include <iostream>
#include <server_manager.hpp>
#include "database_client_impl.hpp"

using namespace std;

class TestBase
{
public:
	virtual ~TestBase()
	{
	}

	TestBase()
	{
		//Proceed();
	}

	void Proceed()
	{
		Test();
	}

	virtual void Test() = 0;
};

class TestImpl : public TestBase
{
public:	

	TestImpl() : TestBase()
	{
		Proceed();
	}

	void Test() override
	{
		std::cout << "here" << std::endl;
	}
};
/*
void callback(shared_ptr<Services::GetResponse> response)
{
	cout << "on response " << endl;
}
*/
int main()
{
	/*
	contracts::services::ServiceAddress address("localhost", 49065);
	grpc_services::ClientContext context(address);
	grpc_services::DatabaseClientImpl impl(context);

	impl.start();
	
	auto gpr = new DataTypes::GetPersonRequest();
	gpr->set_first_name("Test");
	Services::GetRequest gr;
	gr.set_allocated_person_request(gpr);
	
	try
	{
		auto val = impl.get(gr, callback);
		std::cout << val->items().items().size() << std::endl;
	}
	catch ( std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	*/
	//TestImpl im;
		
	grpc_services::ServerManager sm;
	sm.start();
	
	cin.get();
	//sm.stop();

	return 0;
}