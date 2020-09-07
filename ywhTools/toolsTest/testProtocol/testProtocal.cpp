#include "testProtocal.h"





void PatsFixProtocal::decode_data(const unsigned char* data, unsigned int length)
{

}

void PatsFixProtocal::encode_data(std::vector<unsigned char>& buffer, int &send_len, std::queue<YWH_TOOLS::Any> &queue)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	buffer.clear();
	YWH_TOOLS::Any msg;
	std::cout << "queuesize = " << queue.size()<<std::endl;
	int len = queue.size();
	for (int i = 0; i < len; i++) {
		msg = queue.front();
		queue.pop();
		
		if (msg.Is<std::string>())
		{

			std::string ret = msg.AnyCast<std::string>();

			
			
			if (buffer.capacity() < send_len + ret.size())
			{
				std::vector<unsigned char> newBuffer;
				newBuffer.reserve(send_len + ret.size());

				memcpy(newBuffer.data(), buffer.data(), buffer.capacity());

				buffer.swap(newBuffer);
			}
				
			memcpy(buffer.data()+ send_len, ret.c_str(), ret.size());
			send_len += ret.size();


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
				

			memcpy(buffer.data()+ send_len, sr.c_str(), sr.size());

			send_len += sr.size();
		}

		else if (msg.Is<HeartBeat>())
		{
			std::string sr = "Digtal\n";
			YWH_TOOLS::Any msg2 = msg;
			HeartBeat ret = msg2.AnyCast<HeartBeat>();
			sr = ret.data;
			//std::cout << "Int = " << ret << std::endl;
			
			if (buffer.capacity() < send_len + sr.size()) {
				std::vector<unsigned char> newBuffer;
				newBuffer.reserve(send_len + sr.size());
				memcpy(newBuffer.data(), buffer.data(), buffer.capacity());
				buffer.swap(newBuffer);
			}
				
			memcpy(buffer.data()+ send_len, sr.c_str(), sr.size());
			send_len += sr.size();

		}
	}
		
	if (queue.size() != 0)
	{
		std::cout << "err queue.size() != 0 in  PatsFixProtocal::encode_data\n";
	}
}


void PatsFixProtocal::reset()
{

}