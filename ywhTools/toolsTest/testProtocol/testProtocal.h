#include "../../protocol.h"



#include <time.h>
#include <sys/timeb.h>

struct HeartBeat {
	const std::string data = "HeartBeat";
	const int heart = 1;
};

class PatsFixProtocal : public YWH_TOOLS::IProtocol
{

public:
	virtual void decode_data(const unsigned char* data, unsigned int length) override;
	virtual void encode_data(std::vector<unsigned char>& buffer, int &size, std::queue<YWH_TOOLS::Any> &queue) override;
	virtual void reset()override;

private:
	void parseMsgHeaderOld(char flag, unsigned short onelen, unsigned short fiveLen, std::vector<char> &hearder);

};