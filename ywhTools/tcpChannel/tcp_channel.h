#pragma once

#include <thread>
#include <queue>

#include "./tcpCleint/tcp_client.h"
#include "../event.h"
#include "../chananel.h"


class TcpChannel : public TcpClient<>,public YWH_TOOLS::IChannel
{
public:
	TcpChannel(std::shared_ptr<YWH_TOOLS::IProtocol> protocol,const std::string& ip,unsigned int port);
	~TcpChannel();
public:
	virtual bool start();
	virtual void stop();
	virtual bool send_data(const unsigned char* buffer, unsigned int size);
private:
    virtual void onConnection(const asio::error_code& ec);
    virtual void onSendError(const asio::error_code& ec);
    virtual void onReciveError(const asio::error_code& ec);
	virtual void onReceiveComplete(const unsigned char* buffer, size_t readBytes);
private:
	std::mutex lock_;
	bool isConnected_;
	std::mutex sendMsgLock_;
	std::string serverAddr_;
	int port_;
	YWH_TOOLS::Event envent_;
	YWH_TOOLS::Event connect_event_;
};