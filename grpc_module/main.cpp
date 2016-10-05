#include <iostream>
#include <server_manager.hpp>

using namespace std;

class ServerImpl final {
public:
	~ServerImpl() {
		server_->Shutdown();
		// Always shutdown the completion queue after the server.
		cq_->Shutdown();
	}

	// There is no shutdown handling in this code.
	void Run() {
		//std::string server_address("0.0.0.0:50051");
		contracts::services::ServiceAddress sa("0.0.0.0", 50051);
		//service_ = std::make_shared<Services::UnitService::AsyncService>();
		auto builder = std::make_shared<grpc::ServerBuilder>();

		//builder->AddListeningPort(sa.FormattedAddress(), grpc::InsecureServerCredentials());

		auto unit_service = std::make_shared<grpc_services::UnitServiceImpl>(grpc_services::ServiceContext(sa, builder));
	
		
		//builder->RegisterService(service_.get());

		//cq_ = builder->AddCompletionQueue();
		
		server_ = builder->BuildAndStart();
		std::cout << "Server listening on " << sa.FormattedAddress() << std::endl;
		unit_service->Start();
		//HandleRpcs();
	}

private:

	std::shared_ptr<Services::UnitService::AsyncService> service_;

	std::unique_ptr<grpc::ServerCompletionQueue> cq_;

	grpc::ServerContext ctx_;


	enum CallStatus { CREATE, PROCESS, FINISH };
	CallStatus status_; 

	void HandleRpcs() {
		
		new grpc_services::unit_service::CreatePopulationHandler(service_, cq_.get());
		void* tag;  // uniquely identifies a request.
		bool ok;
		while (true) {		
			cq_->Next(&tag, &ok);
			if (ok)		
			  static_cast<grpc_services::unit_service::CreatePopulationHandler*>(tag)->Proceed();
		}
	}
	std::unique_ptr<grpc::Server> server_;
};

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

int main()
{	
	//TestImpl im;
	
	ServerImpl impl;
	impl.Run();
	//grpc_services::ServerManager sm;
	//sm.Start();
	
	//cin.get();
//	sm.Stop();

	return 0;
}