#pragma once
//#include <boost/noncopyable.hpp>

//#include <boost/asio.hpp>
#include <thread>
#include <atomic>

#include <list>
#include <vector>
#include <thread>
#include <chrono>

#include <string.h>
#include <asio.hpp>
#include <boost\thread.hpp>
#define TRADER_SYSTEM_UTIL_TCPCLIENT_DEFAULT_RX_BUFFER_SIZE 4096
#define TRADER_SYSTEM_UTIL_TCPCLIENT_DEFAULT_TX_BUFFER_SIZE 4096


enum TcpClientStatus
{
	TcpClientStatusIdle = 0,
	TcpClientStatusResolvingHost,
	TcpClientStatusConnecting,
	TcpClientStatusConnected,
	TcpClientStatusClosing,
	TcpClientStatusClosed,
	TcpClientStatusError = -1
};

template <unsigned int RxBufferSize = TRADER_SYSTEM_UTIL_TCPCLIENT_DEFAULT_RX_BUFFER_SIZE, unsigned int TxBufferSize = TRADER_SYSTEM_UTIL_TCPCLIENT_DEFAULT_TX_BUFFER_SIZE>
class TcpClient : public std::enable_shared_from_this<TcpClient<RxBufferSize, TxBufferSize>>, private asio::detail::noncopyable {
public:
	TcpClient()
		: io_(/*new boost::asio::io_service()*/)
		, work_(io_)
		, resolver_(io_)
		, socket_(io_)
		, status_(TcpClientStatusIdle)
		, txBuffer_()
	{
		sending_ = false;
		receiving_ = false;
	}

	virtual ~TcpClient()
	{
		if (ioThread_.joinable())
		{
			ioThread_.join();
		}
	}
public:
	virtual void onConnection(const asio::error_code& ec) = 0;
	virtual void onReceiveComplete(const unsigned char* buffer, size_t readBytes) = 0;
	virtual void onSendError(const asio::error_code& ec) = 0;
	virtual void onReciveError(const asio::error_code& ec) = 0;
public:
	void send(const unsigned char* buffer, unsigned int size)
	{
		if (0 == size)
		{
			return;
		}
		{
			std::lock_guard<std::mutex> guard(sendLock_);
			size_t offset = txBuffer_.size();
			txBuffer_.resize(offset + size);
			memcpy(&txBuffer_[offset], buffer, size);
		}
		startTransmit_();
	}
	void send(const std::vector<unsigned char>& buffer)
	{
		if (buffer.empty())
		{
			return;
		}

		{
			std::lock_guard<std::mutex> guard(sendLock_);

			if (txBuffer_.empty())
			{
				txBuffer_ = buffer;
			}
			else
			{
				size_t offset = txBuffer_.size();
				txBuffer_.resize(offset + buffer.size());
				memcpy(&txBuffer_[offset], &buffer[0], buffer.size());
			}
		}
		startTransmit_();
	}

	void close()
	{
		std::lock_guard<std::mutex> guard(lock_);
		if (!socket_.is_open())
		{
			return;
		}
		asio::error_code ec;
		socket_.shutdown(asio::ip::tcp::socket::shutdown_receive);
		socket_.close(ec);;
	}

	asio::ip::tcp::socket* getSocket()
	{
		return &socket_;
	}

private:
	typedef asio::ip::basic_resolver<asio::ip::tcp> resolver;

public:
	bool connect(const std::string& host, int port)
	{

		{
			std::lock_guard<std::mutex> guard(lock_);
			if (status_ != TcpClientStatusIdle)
			{
				std::cout << "status_ != TcpClientStatusIdle in connect tcp_client.h" << std::endl;
				return false;
			}
			//status_ = TcpClientStatusResolvingHost;
		}

		//if (socket_.is_open())
		//	socket_.close();

		std::cout << "start resolver_.async_resolve ";
		resolver::query query(host, std::to_string(port), asio::ip::resolver_query_base::all_matching);

		
	
		resolver_.async_resolve(query, std::bind(&TcpClient::onAddressResolved_, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	


	

		
		if (ioThread_.joinable())
		{	
			std::thread tr;
			
			ioThread_.interrupt();
		}

		ioThread_ = std::move(boost::thread(std::bind(&TcpClient::worker_, this->shared_from_this())));
		
		return true;
	}
private:
	void tryReconnect()
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
		if (!socket_.is_open())
			connect_();
	}
private:
	void onAddressResolved_(const asio::error_code& ec, asio::ip::tcp::resolver::iterator it)
	{

		std::cout << "onAddressResolved_\n";
		if (ec)
		{
			onConnection(ec);
			std::lock_guard<std::mutex> guard(lock_);
			status_ = TcpClientStatusError;
		}
		else
		{
			std::lock_guard<std::mutex> guard(lock_);
			status_ = TcpClientStatusConnecting;
			resolveResult_ = it;
			connect_();
		}
	}

	void notAddressResolved()
	{
		std::lock_guard<std::mutex> guard(lock_);
		status_ = TcpClientStatusConnecting;		
		connect_();
	}

	void connect_()
	{

		resolver::endpoint_type endpoint = resolveResult_->endpoint();
		socket_.async_connect(endpoint, std::bind(&TcpClient::onConnect_, this->shared_from_this(), std::placeholders::_1));
	}

	void onConnect_(const asio::error_code& ec)
	{
		if (ec)
		{
			resolveResult_++;
			resolver::iterator end;
			if (resolveResult_ == end)
			{
				status_ = TcpClientStatusError;
				status_ = TcpClientStatusIdle;
				onConnection(ec);
				std::lock_guard<std::mutex> guard(lock_);


			}
			else
			{
				asio::error_code ec;
				socket_.close(ec);
				connect_();
			}
		}
		else
		{
			onConnection(ec);
			{
				std::lock_guard<std::mutex> guard(sendLock_);
				sending_ = false;
			}
			std::lock_guard<std::mutex> guard(lock_);
			status_ = TcpClientStatusConnected;
			startReceive_();
		}
	}

private:
	void startTransmit_()
	{
		{
			std::lock_guard<std::mutex> guard(sendLock_);
			if (sending_)
			{
				return;
			}
			if (!txBuffer_.size())
			{
				return;
			}
			sending_ = true;
		}
		transmit_();
	}

	void transmit_()
	{
		std::lock_guard<std::mutex> guard(sendLock_);
		transferredInBuffer_ = 0;
		transferingBuffer_ = txBuffer_;
		nativeTxBuffer_ = &(transferingBuffer_)[0];
		txBuffer_.clear();

		doTransmit_();
	}

	void doTransmit_()
	{
		socket_.async_write_some(
			asio::buffer(nativeTxBuffer_ + transferredInBuffer_, transferingBuffer_.size() - transferredInBuffer_),
			std::bind(&TcpClient::onTransmitComplete_, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2)
		);
	}

	void onTransmitComplete_(const asio::error_code& ec, size_t written)
	{
		if (ec)
		{
			status_ = TcpClientStatusIdle;
			close();
			onSendError(ec);
			{
				std::lock_guard<std::mutex> guard(sendLock_);
				sending_ = false;
			}


			return;
		}

		transferredInBuffer_ += written;

		if (transferredInBuffer_ == transferingBuffer_.size())
		{
			{
				std::lock_guard<std::mutex> guard(sendLock_);
				sending_ = false;
			}
			startTransmit_();
		}
		else
		{
			doTransmit_();
		}
	}

private:
	void startReceive_()
	{
		{
			std::lock_guard<std::mutex> guard(rxLock_);

			if (receiving_)
			{
				return;
			}
			receiving_ = true;
		}
		receive_();
	}

	void receive_()
	{
		socket_.async_read_some(
			asio::buffer(rxBlock_, RxBufferSize),
			std::bind(&TcpClient::onReceiveComplete_, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2)
		);
	}

	void onReceiveComplete_(const asio::error_code& ec, size_t readBytes)
	{
		if (ec)
		{

			close();
			status_ = TcpClientStatusIdle;
			onReciveError(ec);

			return;
		}

		if (readBytes)
		{
			onReceiveComplete(rxBlock_, readBytes);
		}

		receive_();
	}
	const  asio::ip::tcp::socket *getTcpSocket()
	{
		return (&socket_);
	}

private:
	void worker_()
	{

		try
		{
			std::cout << "io run!\n";
			io_.run();
			std::cout << "io stop!\n" << std::endl;
			return;
		}
		catch (...)
		{
			int x = 0;
			x++;
			std::cout << "io err!\n" << std::endl;
			return;
		}
		int i = 0;
		i++;
	}

	void worker_loop()
	{
		while (workerRuning)
		{

			try
			{
				std::cout << "io run!\n";
				io_.run();
				std::cout << "io stop!\n" << std::endl;
				return;
			}
			catch (...)
			{
				int x = 0;
				x++;
				std::cout << "io err!\n" << std::endl;
				return;
			}
			int i = 0;
			i++;
		}
	}

private:
	asio::io_service io_;
	asio::io_service::work work_;
	boost::thread ioThread_;
	asio::ip::tcp::resolver resolver_;
	asio::ip::tcp::resolver *resolverPtr = nullptr;
	asio::ip::tcp::socket socket_;
	asio::ip::tcp::resolver::iterator resolveResult_;

	std::mutex lock_;

	TcpClientStatus status_;

	std::atomic_bool sending_;
	std::atomic_bool receiving_;

	std::mutex sendLock_;
	std::vector<unsigned char> txBuffer_;
	std::vector<unsigned char> transferingBuffer_;
	unsigned char* nativeTxBuffer_;
	size_t transferredInBuffer_;
	unsigned char rxBlock_[RxBufferSize];
	std::mutex rxLock_;
	std::atomic_bool workerRuning;
};



