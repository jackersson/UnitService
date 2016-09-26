#ifndef Threadable_Included
#define Threadable_Included

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace utils
{
	class threadable
	{
	public:
		threadable() : active_(false)
		{
			threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
		}

		virtual ~threadable() {}
		
		void start()
		{
			if (active_)
				return;

			io_service.post(boost::bind(thread_procedure, this));
		}

		virtual void stop()
		{
			active_ = false;
			io_service.stop();
			threads.join_all();
		}

		bool active() const {
			return active_;
		}

	protected:
		void virtual run() = 0;

	private:
		static void thread_procedure(void* thread_context)
		{
			auto instance = static_cast<threadable*>(thread_context);

			if (instance == nullptr)
				return;
			instance->active_ = true;
			instance->run();
			instance->active_ = false;
		}

		boost::asio::io_service io_service;
		boost::thread_group threads;

		bool active_;
	};
}

#endif