#include "../Any.h"
#include "../endpoint.h"
#include "testProtocol\testProtocal.h"
#include "../tcpChannel/tcp_channel.h"

std::shared_ptr<YWH_TOOLS::Endpoint>	tcpEndPoint=nullptr;

int testAny()
{
	struct As {
		int i;
		std::string str;
	};
	class Bs {
	public:
		std::string str;
		int i;
	};

	As as;//or    Bs as;
	as.i = 1;
	as.str = "str222";
	YWH_TOOLS::Any aa = YWH_TOOLS::Any(as);



	if (aa.Is<Bs>())
	{
		std::cout << "is bs\n";
		Bs an = aa.AnyCast<Bs>();
		std::cout << an.i << "--" << an.str.c_str() << std::endl;
	}
	else if (aa.Is<As>()) {
		std::cout << "is as\n";
		As an = aa.AnyCast<As>();
		std::cout << an.i << "--" << an.str.c_str() << std::endl;
	}

	return 0;
}
void tcpMessageRecieve(unsigned int, const YWH_TOOLS::Any& msg)
{
	std::cout << "Tcp receive msg: "<< std::endl;
	return;
}

void endPointHeart(std::shared_ptr<YWH_TOOLS::Endpoint> ep)
{
	HeartBeat hb;

	while (1) {
		if (ep == nullptr)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		ep->send_msg(YWH_TOOLS::Any(hb));

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void endpointStartThread(std::shared_ptr<YWH_TOOLS::Endpoint> ep)
{
	ep->start();
	endPointHeart(ep);
}
void testEndpointAndTcpChannel()
{
	using namespace YWH_TOOLS;
	std::shared_ptr<IChannel> tcpChannel=std::make_shared<TcpChannel> 
		(std::make_shared<PatsFixProtocal>(),
		std::string("192.168.1.168"), 10018);

	tcpEndPoint = std::shared_ptr<Endpoint>(new Endpoint());

	tcpEndPoint->bind(tcpChannel, tcpMessageRecieve);

	//tcpEndPoint->start();
	std::thread tr(std::bind(endpointStartThread, tcpEndPoint));

	while (1)
	{
		std::string str = "str msg\n";
		int i = 9527;
		static int k=0;
		k++;
		if (k% 2) {
			tcpEndPoint->send_msg(Any(str));
		}
		else {
			tcpEndPoint->send_msg(Any(i));
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		
	}


}