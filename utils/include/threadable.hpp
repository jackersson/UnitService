#ifndef Threadable_Included
#define Threadable_Included
#include <thread>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace utils
{
	class Threadable
	{
	public:
		Threadable() : active_(false), cancelation_requested(false)
			, work(io_service)
		{
			threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
		}

		virtual ~Threadable() {}
		
		void start()
		{
			if (active_)
				return;

			//thread_ = std::thread(&Threadable::thread_procedure, this);
			io_service.post(boost::bind(thread_procedure, this));
		}

		virtual void stop()
		{
			cancelation_requested = true;
			active_ = false;
			io_service.stop();
			threads.join_all();
		}

		bool active() const {
			return active_;
		}

	protected:
		void virtual run() = 0;

		bool cancelation_requested;
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

		//std::thread thread_;

		boost::asio::io_service io_service;
	  boost::thread_group threads;
		boost::asio::io_service::work work;

		bool active_;

	};
}

#endif