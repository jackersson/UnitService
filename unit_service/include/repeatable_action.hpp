#ifndef RepeatableAction_Included
#define RepeatableAction_Included

#include <threadable.hpp>

class IRepeatableAction
{
public:
	virtual ~IRepeatableAction()
	{}

	virtual void action() = 0;
	virtual bool can() = 0;

};

class RepeatableAction : public utils::Threadable
{
public:
	explicit RepeatableAction( IRepeatableAction* action		                      
	                         , std::chrono::seconds delay )
		                       :	action_(action), delay_(delay)
	{}

	virtual ~RepeatableAction() {
		Threadable::stop();
	}

	void run() override
	{
		while (active())
		{
			std::this_thread::sleep_for(delay_);

			if (action_ != nullptr && action_->can())
				action_->action(); 

			if (cancelation_requested)				
				break;
		}
	}

private:
	IRepeatableAction* action_;	
	std::chrono::seconds delay_ = std::chrono::seconds(3);
};

#endif