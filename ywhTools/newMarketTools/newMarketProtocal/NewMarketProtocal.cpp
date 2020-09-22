#include "NewMarketProtocal.h"
#include "../marketType/marketType.h"
//#include "../../log/log.h"

namespace YWH_TOOLS 
{


	void NewMarketProtocal::decode_data(const unsigned char* data, unsigned int length)
	{

	}

	void NewMarketProtocal::encode_data(std::vector<unsigned char>& buffer, int &send_len, std::queue<YWH_TOOLS::Any> &queue)
	{

		buffer.clear();
		send_len = 0;

		const int headLen=12;
		int tcpLen = 2;
		static std::vector<char> tcpLenVc(tcpLen);
		static std::vector<char> msgHeader(headLen);

		YWH_TOOLS::Any msg;
		std::cout << "测试 queuesize = " << queue.size() << std::endl;
		int len = queue.size();

		for (int i = 0; i < len; i++) {
			msg = queue.front();
			queue.pop();
			
			if (msg.Is<SendMarketData>())
			{

				SendMarketData ret = msg.AnyCast<SendMarketData>();

				parseMsgHeader((char)(ret.msgType), ret.oneLen, ret.fiveLen, msgHeader);

				if (ret.useTcp)
					tcpLen = 2;
				else
					tcpLen = 0;

				int tcpBodyLen = headLen + ret.data.size();
				int appendLen= tcpLen + headLen + ret.data.size();

				if (buffer.capacity() < appendLen+ send_len)
				{
					std::vector<unsigned char> newBuffer;
					newBuffer.reserve(send_len + appendLen);

					memcpy(newBuffer.data(), buffer.data(), buffer.capacity());
					buffer.swap(newBuffer);
				}

				//复制tcp头
				if (ret.useTcp) {
					if (tcpBodyLen > 255) {
						std::cout << "There is something wrong tcpBodyLen > 255 \n";
					}
					memcpy(tcpLenVc.data(), (char *)(&tcpBodyLen), 2);
					char t = tcpLenVc[0];
					tcpLenVc[0] = tcpLenVc[1];
					tcpLenVc[1] = t;

					memcpy(buffer.data() + send_len, tcpLenVc.data(), 2);
					send_len = send_len + 2;
				}

				//复制12字节固定大小head
				memcpy(buffer.data() + send_len, msgHeader.data(), headLen);
				send_len += headLen;

				//复制内容
				memcpy(buffer.data() + send_len,ret.data.c_str(), ret.data.size());
				send_len += ret.data.size();


			}
			else if (msg.Is<int>())
			{
				std::string sr = "Digtal\n";


				if (buffer.capacity() < send_len + sr.size()) {
					std::vector<unsigned char> newBuffer;
					newBuffer.reserve(send_len + sr.size());
					memcpy(newBuffer.data(), buffer.data(), buffer.capacity());
					buffer.swap(newBuffer);
				}


				memcpy(buffer.data() + send_len, sr.c_str(), sr.size());

				send_len += sr.size();
			}


		}

		if (queue.size() != 0)
		{
			std::cout << "err queue.size() != 0 in  PatsFixProtocal::encode_data\n";
		}
	}


	void NewMarketProtocal::reset()
	{

	}

	void NewMarketProtocal::parseMsgHeader(char flag, unsigned short onelen, unsigned short fiveLen, std::vector<char> &header)
	{


		if (header.size() < 12)
			header.resize(12);
		//BYTE header[12] = {};
		memset(&header[0], 0, 12);
		header[0] = marketVersion;
		header[1] = flag;
		if (onelen == 0)
			header[3] = '-';
		else
			header[3] = (unsigned char)(onelen + 12);

		if (fiveLen == 0)
			header[2] = '-';
		else
			header[2] = (unsigned char)(fiveLen + 12);


		struct timeb t_;
		ftime(&t_);

		long long mills = t_.time * 1000 + t_.millitm;

		//时间戳小端存储
		unsigned char temp[8] = {};
		memset(temp, 0, 8);
		memcpy(temp, &mills, 8);


		for (int i = 0; i < 8; i++)
		{
			memcpy(&header[i + 4], &temp[i], 1);
		}


	}

}//end namespace
