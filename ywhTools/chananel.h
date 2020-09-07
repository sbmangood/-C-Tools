#pragma once

#include <queue>
#include <thread>

#include "eventDispatcher.h"
#include "event.h"
#include "protocol.h"
#include "any.h"

namespace YWH_TOOLS
{ 

class IChannel: public EventDispatcher<const Any&>
{
public:
	IChannel(std::shared_ptr<IProtocol> protocol);
	virtual ~IChannel();
public:
	virtual bool start() = 0;
	virtual void stop();
	virtual bool send_data(const unsigned char* buffer, unsigned int size) = 0;
	
	
	virtual void async_send(const Any& msg);

	//接收数据的事件通知函数
	void onDataArrived(const unsigned char* buffer, size_t size);

protected:
	virtual int handlerMsgDecoded(unsigned int eventid, EventDispatcher< const Any& > & endpoint, const Any& msg);
	virtual int handlerMsgCorrupted(unsigned int eventid, EventDispatcher< const Any& > & endpoint, const Any& msg);
private:
	void send_work();
protected:
	bool _sending;
	std::shared_ptr<IProtocol> protocol_;
	std::mutex msg_queue_lock;
	std::queue<Any> _msg_queue;
	std::queue<Any> _msg_queue_swap;
	std::shared_ptr<std::thread> _send_thread;
	Event send_event_;
	std::vector<unsigned char> buffer;
	const int waitTimeoutMs = 5000;

};

}