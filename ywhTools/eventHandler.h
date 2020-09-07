#pragma once
#include <string>



template<typename MessageT>
class EventDispatcher;

template<typename MessageT>
class IEventHandler
{
public:
	IEventHandler(unsigned int eventType, unsigned int priority = 0)
		: _eventType(eventType)
		, _priority(priority)
	{

	}

	virtual ~IEventHandler()
	{

	}

	virtual int handle(EventDispatcher<MessageT>& sender, MessageT message) = 0;

	unsigned int getEventType()
	{
		return _eventType;
	}
protected:
	unsigned int _eventType;
	unsigned int _priority;
};

template<typename HostT, typename MessageT>
class EventHandler : public IEventHandler<MessageT>
{
public:
	typedef int(HostT::*HostFunc) (unsigned int eventid, EventDispatcher<MessageT>& sender, MessageT message);

	EventHandler(HostT& host, HostFunc func, unsigned int eventType, unsigned int priority = 0, const char* handlerName = "_unnamed", bool noAutoRelease = false)
		: IEventHandler<MessageT>(eventType, priority)
		, _host(&host), _func(func)
	{

	}

	virtual ~EventHandler()
	{

	}

	virtual int handle(EventDispatcher<MessageT>& sender, MessageT message)
	{
		return (_host->*_func)(this->_eventType, sender, message);
	}

protected:
	HostT* _host;
	HostFunc _func;
};

