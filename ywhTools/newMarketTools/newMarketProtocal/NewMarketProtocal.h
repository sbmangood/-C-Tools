#pragma once

#include "../../protocol.h"


namespace YWH_TOOLS {
	class NewMarketProtocal : public IProtocol
	{

	public:
		virtual void decode_data(const unsigned char* data, unsigned int length) override;
		virtual void encode_data(std::vector<unsigned char>& buffer, int &size, std::queue<YWH_TOOLS::Any> &queue) override;
		virtual void reset()override;

	private:
		void parseMsgHeader(char flag, unsigned short onelen, unsigned short fiveLen, std::vector<char> &header);

	};
}

