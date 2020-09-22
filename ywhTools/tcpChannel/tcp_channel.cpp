#include "tcp_channel.h"
#include <iterator>
#include <algorithm>


#include "../log/log.h"


static LOGGER logger = LoggerUtils::get_mutable_instance().getLogger("TraderServer.TcpChannel");

TcpChannel::TcpChannel(std::shared_ptr<YWH_TOOLS::IProtocol> protocol, const std::string& ip, unsigned int port):
	IChannel(protocol)
    ,isConnected_(false)
	,serverAddr_(ip)
	, port_(port)
{

}

TcpChannel::~TcpChannel()
{

}

void TcpChannel::onConnection(const asio::error_code& ec)
{
    isConnected_ = !ec ? true : false;
    //!ec ? logger->info("connect sucess") : 
    //    logger->error("connect failed {}",ec.message().c_str());
	if (!ec) {
		
		logger->debug("connect sucess!");
	}
	else {
		logger->debug("connect failed! {}", ec.message().c_str());
	
	}

	
    connect_event_.set();
}

void TcpChannel::onSendError(const asio::error_code& ec)
{
    //logger->error("send message to ZDTradeCommunication failed, {}", ec.message().c_str());
    std::string  cmd="onSendError";
	logger->error("send message to ZDTradeCommunication failed,{}", ec.message().c_str());
	isConnected_ = false;
	connect_event_.set();
    this->dispatchMessage(YWH_TOOLS::IProtocol::EVENT_MSG_ARRIVED, YWH_TOOLS::Any(cmd));

	std::this_thread::sleep_for(std::chrono::microseconds(500));
	start();
}

void TcpChannel::onReciveError(const asio::error_code& ec)
{
    logger->error("receive message from ZDTradeCommunication failed, {},code is {}", ec.message().c_str(),ec.value());
	std::string  cmd = "onSendError";


    this->dispatchMessage(YWH_TOOLS::IProtocol::EVENT_MSG_ARRIVED, YWH_TOOLS::Any(cmd));
	
	isConnected_ = false;
	connect_event_.set();

	std::this_thread::sleep_for(std::chrono::microseconds(500));
	start();
	
}

void TcpChannel::onReceiveComplete(const unsigned char* buffer, size_t readBytes)
{
	IChannel::onDataArrived(buffer, readBytes);
	
}

bool TcpChannel::start()
{
	/*
	if (!connect(serverAddr_, port_))
	{
		return false;
	}
	*/
	
	auto tid = std::this_thread::get_id();
	
	connect(serverAddr_, port_);
	
	static int cnt = 1;
	//connect_event_.wait();
	while (!isConnected_)
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
		
		//logger->error("Tcp socket Reconect({})!", cnt++);
		connect(serverAddr_, port_);
		//connect_event_.wait();
	}
	cnt = 1;
	return isConnected_;
}

void TcpChannel::stop()
{
	IChannel::stop();
	close();
}

bool TcpChannel::send_data(const unsigned char* buffer, unsigned int size)
{
	if (!isConnected_)
	{
		logger->error("Tcp socket is not connect!");
	
		return false;
	}
	try
	{
		send(buffer, size);
	}
	catch(...)
	{
		logger->error("known err in send_data");
		
	}
	return true;
}