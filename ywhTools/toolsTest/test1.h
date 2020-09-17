#include "../Any.h"
#include "../endpoint.h"
#include "testProtocol\testProtocal.h"
#include "../tcpChannel/tcp_channel.h"
#include "../newMarketTools/newMarketProtocal/NewMarketProtocal.h"
#include "../newMarketTools/marketType/marketType.h"
using namespace YWH_TOOLS;
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

		std::this_thread::sleep_for(std::chrono::seconds(hb.heart));
	}
}
void endpointStartThread(std::shared_ptr<YWH_TOOLS::Endpoint> ep)
{
	ep->start();
	//endPointHeart(ep);
}
void testEndpointAndTcpChannel()
{
	using namespace YWH_TOOLS;
	std::shared_ptr<IChannel> tcpChannel=std::make_shared<TcpChannel> 
		(std::make_shared<NewMarketProtocal>(),
		std::string("127.0.0.1"), 10010);

	tcpEndPoint = std::shared_ptr<Endpoint>(new Endpoint());

	tcpEndPoint->bind(tcpChannel, tcpMessageRecieve);


	std::thread tr(std::bind(endpointStartThread, tcpEndPoint));

	std::string str = "str msg\n";
	int i = 9527;
	static int k = 0;
	SendMarketData tj1;
	SendMarketData tj2;
	tj1.msgType = MSG_SEND_HIGH_OPEN_LOW;
	tj2.msgType = MSG_SEND_HIGH_OPEN_LOW;
	tj1.data = "XEurex@FBON2106@77@77@7@4@5@5@9999900.000@0.020@3";
	tj2.data = "XEurex@FBON2106@5@7@@7@7@7@888900.000@0.020@3";

	SendMarketData cj1;
	SendMarketData cj2;//MSG_SEND_DEAL////XEurex@FOAT2012@5,1,2,,8,@16814@144@16816@153@168.15@3@22438@@187@@230@@135@@143@@147@@156@@147@@146
	cj1.msgType = MSG_SEND_DEAL;
	cj2.msgType = MSG_SEND_DEAL;
	cj1.data = "XEurex@FBON2106@168.15@24@22438@@";
	cj2.data = "XEurex@FBON2106@168.15@21@22437@@";

	SendMarketData pk1;
	SendMarketData pk2;
	pk1.msgType = MSG_SEND_PRICE_DEPTH;
	pk2.msgType = MSG_SEND_PRICE_DEPTH;
	pk1.data = "XEurex@FBON2106@5,1,2,,8,@16814@144@16816@1@168.15@3@22438@@2@@23@@15@@143@@17@@16@@47@@146";
	pk2.data = "XEurex@FBON2106@5,1,2,,8,@16814@144@16816@153@168.15@3@22438@@187@@23@@15@@143@@147@@156@@14@@16";


	while (1)
	{

		k++;
		if (k% 2) {
			tcpEndPoint->send_msg(Any(tj1));
			tcpEndPoint->send_msg(Any(cj1));
			tcpEndPoint->send_msg(Any(pk1));
		}
		else {
			tcpEndPoint->send_msg(Any(tj2));
			tcpEndPoint->send_msg(Any(cj2));
			tcpEndPoint->send_msg(Any(pk2));
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		
	}

	
}

void testDump() {
	int b = 0;
	int a = 1;
	int c = a / b;
}