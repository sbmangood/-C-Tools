#pragma once

#include "any.h"
#include "eventDispatcher.h"
#include <vector>
#include <queue>

namespace YWH_TOOLS
{

class IProtocol : public EventDispatcher<const Any&>
{
public:
	enum 
	{
		EVENT_MSG_ARRIVED = 0,
		EVENT_MSG_CORRUPTED,
	};
public:
	IProtocol()
	{

	}
	virtual ~IProtocol()
	{

	}
public:
	virtual void decode_data(const unsigned char* data,unsigned int length) = 0;
	virtual void encode_data(const Any& message, std::vector<unsigned char>& buffer)
	{}//no use
	virtual void encode_data(const Any& message, std::vector<unsigned char>& buffer, int &size)
	{}//no use
	virtual void encode_data( std::vector<unsigned char>& buffer, int &size, std::queue<Any> &queue) = 0;
	
	virtual void reset() = 0;
private:
};


}