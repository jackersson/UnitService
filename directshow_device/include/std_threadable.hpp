#ifndef StdThreadable_Included
#define StdThreadable_Included
#include <thread>

namespace utils
{
	class StdThreadable
	{
	public:
		StdThreadable() : cancelation_requested(false), active_(false)
		{}

		virtual ~StdThreadable() {}

		void start()
		{
			if (active_)
				return;

			thread_ = std::thread(&StdThreadable::thread_procedure, this);
		}

		virtual void stop()
		{
			cancelation_requested = true;
			active_ = false;
			thread_.join();
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
			auto instance = static_cast<StdThreadable*>(thread_context);

			if (instance == nullptr)
				return;
			instance->active_ = true;
			instance->run();
			instance->active_ = false;
		}
		
		std::thread thread_;
		bool active_;

	};
}

#endif