#pragma once

#include <map>
#include <iostream>
#include <mutex>
#include "eventHandler.h"

template<typename MessageT>
class EventDispatcher
{
public:
	EventDispatcher()
	{
		removeAllListeners();
	}

	virtual ~EventDispatcher()
	{
		removeAllListeners();
	}

	void removeAllListeners()
	{
		std::lock_guard<std::mutex> lock(mapLocker_);
		typename std::multimap<unsigned int, IEventHandler<MessageT>* >::iterator iter = _handlerMap.begin();
		for (; iter != _handlerMap.end(); iter++)
		{
			ReleaseListenerHandler(iter->second);
		}
		_handlerMap.clear();
	}

	void removeListeners(unsigned int eventType)
	{

		std::lock_guard<std::mutex> guard(mapLocker_);
		typename multimap<unsigned int, IEventHandler<MessageT>*>::iterator iter = _handlerMap.lower_bound(eventType);
		for (; iter != _handlerMap.upper_bound(eventType); iter++)
		{
			delete iter->second;
		}

		iter = _handlerMap.find(eventType);
		_handlerMap.erase(iter, _handlerMap.end());
	}

	template <class HostT>
	static IEventHandler<MessageT>*  CreateListenerHandler(
		unsigned int eventId,
		int(HostT::*func) (unsigned int eventid, EventDispatcher<MessageT>& sender, MessageT message),
		HostT* pThis, unsigned int priority = 0)
	{
		IEventHandler<MessageT>* eventHandler = new EventHandler<HostT, MessageT>(*pThis, func, eventId, priority);
		return eventHandler;
	}


	static void ReleaseListenerHandler(IEventHandler<MessageT>* handler)
	{
		delete handler;
	}

	template <typename HostT>
	IEventHandler<MessageT>*  addListener(
		unsigned int eventId,
		int(HostT::*func) (unsigned int eventid, EventDispatcher<MessageT>& sender, MessageT message),
		HostT* pThis, unsigned int priority = 0)
	{

		IEventHandler<MessageT>* eventHandler = CreateListenerHandler<HostT>(eventId, func, pThis, priority);
		if (registerEventHandler(eventHandler))
		{
			return eventHandler;
		}
		else
		{
			ReleaseListenerHandler(eventHandler);
			return NULL;
		}
	}

	bool  registerEventHandler(IEventHandler<MessageT>* handler)
	{
		std::lock_guard<std::mutex> lock(mapLocker_);
		_handlerMap.insert(std::pair<unsigned int, IEventHandler<MessageT>*>(handler->getEventType(), handler));
		return true;
	}

	bool  unregisterEventHandler(IEventHandler<MessageT>* handler)
	{
		std::lock_guard<std::mutex> lock(mapLocker_);
		typename multimap<unsigned int, IEventHandler<MessageT>*>::iterator iter = _handlerMap.find(handler->getEventType());
		for (; iter != _handlerMap.end(); iter++)
		{
			if (iter->second == handler)
			{
				ReleaseListenerHandler(iter->second);
				_handlerMap.erase(iter);
				break;
			}
		}
		return true;
	}

	void dispatchMessage(unsigned int eventId, MessageT message)
	{
		std::lock_guard<std::mutex> lock(mapLocker_);
		typename std::multimap<unsigned int, IEventHandler<MessageT>*>::iterator iter = _handlerMap.lower_bound(eventId);
		for (; iter != _handlerMap.upper_bound(eventId); iter++)
		{
			iter->second->handle(*this, message);
		}
	}

	size_t getListenerCount(unsigned int eventId)
	{
		std::lock_guard<std::mutex> lock(mapLocker_);
		size_t ans = _handlerMap.count(eventId);
		return ans;
	}
protected:
	std::multimap<unsigned int, IEventHandler<MessageT>*> _handlerMap;
	std::mutex mapLocker_;
};
