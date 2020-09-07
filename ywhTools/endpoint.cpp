#include "endpoint.h"
namespace YWH_TOOLS
{

	Endpoint::Endpoint()
	{

	}

	Endpoint::~Endpoint()
	{

	}

	bool Endpoint::bind(std::shared_ptr<IChannel> chananel, callbackHandler callFun)
	{
		if (!chananel)
		{
			return false;
		}
		chnanel_ = chananel;
		//messgae_Arrivaled_.connect(callFun);
		//chnanel_->addListener(IProtocol::EVENT_MSG_ARRIVED, &Endpoint::handlerMsgDecoded, this);
		//chnanel_->addListener(IProtocol::EVENT_MSG_CORRUPTED, &Endpoint::handlerMsgDecoded, this);
		return true;
	}



	int Endpoint::handlerMsgDecoded(unsigned int eventid, EventDispatcher< const Any& > & endpoint, const Any& msg)
	{
		//if (!messgae_Arrivaled_.empty())
		//{
		//	messgae_Arrivaled_(eventid, msg);
		//}
		return 0;
	}

	bool Endpoint::start()
	{
		if (!chnanel_)
		{
			return false;
		}
		return chnanel_->start();
	}

	void Endpoint::stop()
	{
		if (chnanel_)
		{
			chnanel_->stop();
		}
	}

	void Endpoint::send_msg(const Any& msg)
	{
		if (chnanel_)
		{
			chnanel_->async_send(msg);
		}
	}



}