#ifndef Threadable_Included
#define Threadable_Included
#include <thread>

//#include <boost/asio.hpp>
//#include <boost/thread.hpp>

namespace utils
{
	class Threadable
	{
	public:
		Threadable() : active_(false)
		{
			//threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
		}

		virtual ~Threadable() {}
		
		void start()
		{
			if (active_)
				return;

			thread_ = std::thread(&Threadable::thread_procedure, this);
			//o_service.post(boost::bind(thread_procedure, this));
		}

		virtual void stop()
		{
			active_ = false;
		//	io_service.stop();
			thread_.join();
		}

		bool active() const {
			return active_;
		}

	protected:
		void virtual run() = 0;

	private:
		static void thread_procedure(void* thread_context)
		{
			auto instance = static_cast<Threadable*>(thread_context);

			if (instance == nullptr)
				return;
			instance->active_ = true;
			instance->run();
			instance->active_ = false;
		}

		std::thread thread_;

		//boost::asio::io_service io_service;
		//boost::thread_group threads;

		bool active_;
	};
}

#endif