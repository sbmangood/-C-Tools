#pragma once
#include <condition_variable>
#include <mutex>
namespace YWH_TOOLS
{
	class Event
	{
	public:
		Event();
		~Event();
	public:
		enum
		{
			Event_Time_Out = 0,
			Event_Ok,
		};
	public:
		int wait(unsigned long long timeOut = 0xffffffffffffffLL);
		void set();
		void set_one();
	private:
		std::mutex mtx;
		std::condition_variable cond;
	};

}