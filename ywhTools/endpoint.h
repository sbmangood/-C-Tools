#pragma once

#include "chananel.h"
#include <csignal>
namespace YWH_TOOLS
{

	class Endpoint
	{
	public:
		Endpoint();
		~Endpoint();
	public:
		typedef std::function<void(unsigned int, const Any&)> callbackHandler;
	public:
		bool bind(std::shared_ptr<IChannel> chnanel, callbackHandler callFun);
		bool start();
		void stop();
		void send_msg(const Any& msg);
	protected:
		int handlerMsgDecoded(unsigned int eventid, EventDispatcher< const Any& > & endpointm, const Any& msg);
	private:
		std::shared_ptr<IChannel> chnanel_;
		//std::signal<void(unsigned int, const Any&)> messgae_Arrivaled_;
	};



}
