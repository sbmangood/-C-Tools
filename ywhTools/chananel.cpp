#pragma
#include "chananel.h"

#include<string>

namespace YWH_TOOLS
{


	IChannel::IChannel(std::shared_ptr<IProtocol> protocol) :
		protocol_(protocol)
	{
		_sending = true;
		_send_thread = std::shared_ptr<std::thread>(new std::thread(std::bind(&IChannel::send_work, this)));
		protocol_->addListener(IProtocol::EVENT_MSG_ARRIVED, &IChannel::handlerMsgDecoded, this);
		protocol_->addListener(IProtocol::EVENT_MSG_CORRUPTED, &IChannel::handlerMsgCorrupted, this);
		buffer.reserve(bufferDefaultSize);
		
	}

	IChannel::~IChannel()
	{

	}

	int IChannel::handlerMsgDecoded(unsigned int eventid, EventDispatcher< const Any& > & endpoint, const Any& msg)
	{
		dispatchMessage(eventid, msg);
		return 0;
	}

	int IChannel::handlerMsgCorrupted(unsigned int eventid, EventDispatcher< const Any& > & endpoint, const Any& msg)
	{
		dispatchMessage(eventid, msg);
		return 0;
	}

	void IChannel::onDataArrived(const unsigned char* buffer, size_t size)
	{
		//logger->info(std::string((char*)buffer,size).c_str());
		protocol_->decode_data(buffer, size);
	}

	void IChannel::async_send(const Any& msg)
	{
		try
		{
			std::lock_guard<std::mutex> lock(msg_queue_lock);
			_msg_queue.push(msg);
			send_event_.set();
		}
		catch (const std::exception& e)
		{
		}
		catch (...)
		{

		}
	}

	void IChannel::send_work()
	{
		//buffer.reserve(1000);
		int sendSize;
		while (_sending)
		{
			{
				send_event_.wait(waitTimeoutMs);
				{
					{
						std::lock_guard<std::mutex> lk(msg_queue_lock);
						if (_msg_queue.empty())
							continue;
						_msg_queue.swap(_msg_queue_swap);
					}
					
					sendSize = 0;
					protocol_->encode_data(buffer, sendSize, _msg_queue_swap);
					send_data(buffer.data(), sendSize);
				}
			}
		}
	}

	void IChannel::stop()
	{
		_sending = false;
		if (_send_thread->joinable())
		{
			_send_thread->join();
		}
	}



}