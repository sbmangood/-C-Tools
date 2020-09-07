#include "event.h"
namespace YWH_TOOLS
{

	Event::Event()
	{

	}

	Event::~Event()
	{

	}

	int Event::wait(unsigned long long timeOutInS)
	{
		std::chrono::milliseconds timeOut(timeOutInS);
		std::unique_lock<std::mutex> lock(mtx);
		return cond.wait_for(lock, timeOut) == std::cv_status::no_timeout ? Event_Ok : Event_Time_Out;
	}

	void Event::set()
	{
		cond.notify_all();
	}

	void Event::set_one()
	{
		cond.notify_one();
	}


}