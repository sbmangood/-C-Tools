#include "MarketDepth.h"

namespace YWH_TOOLS
{

	MarketDepth::MarketDepth()
	{
		askDepth.reserve(depthLen);
		bidDepth.reserve(depthLen);
		impliedAskDepth.reserve(impliedLen);
		impliedBidDepth.reserve(impliedLen);
	}

	void MarketDepth::clear() {
		askDepth.clear();
		bidDepth.clear();
		impliedAskDepth.clear();
		impliedBidDepth.clear();
	}

	bool MarketDepth::pushAskDepth(int price, int vol)
	{
		if (askDepth.size() >= depthLen)
			return false;
		if (price <=0 || vol <= 0)
			return false;
		if (askDepth.size() > 0 && price >= askDepth.back().price)
			return false;
		askDepth.push_back(Depth(price, vol));
		return true;
	}
	bool MarketDepth::pushBidDepth(int price, int vol)
	{
		if (bidDepth.size() >=depthLen)
			return false;
		if (price <= 0 || vol <= 0)
			return false;
		if (bidDepth.size() > 0 && price <= bidDepth.back().price)
			return false;
		bidDepth.push_back(Depth(price, vol));
		return true;
	}
	bool MarketDepth::pushImpliedAskDepth(int price, int vol)
	{
		if (impliedAskDepth.size() >= impliedLen)
			return false;
		if (price <=0 || vol <= 0)
			return false;
		impliedAskDepth.push_back(Depth(price, vol));
		return true;
	}
	bool  MarketDepth::pushImpliedBidDepth(int price, int vol)
	{
		if (impliedBidDepth.size() >= impliedLen)
			return false;
		if (price <=0 || vol <= 0)
			return false;
		impliedBidDepth.push_back(Depth(price, vol));
		return true;
	}


	void MarketDepth::filleImpliedAskDepth()
	{
		//暂时只对买隐含价一进行处理
		if (impliedAskDepth.size() > 0 && askDepth.size()>0)
		{
			if (impliedAskDepth[0].price == askDepth[0].price) {
				askDepth[0].vol += impliedAskDepth[0].vol;
			}
			else if (impliedAskDepth[0].price > askDepth[0].price) {
				askDepth.insert(askDepth.begin(), impliedAskDepth[0]);
			}
			else {
				if (impliedAskDepth.size() <= 1)
					return;
			}

			if (impliedAskDepth[0].price == askDepth[1].price) {
				askDepth[1].vol += impliedAskDepth[0].vol;
			}
			else if (impliedAskDepth[0].price > askDepth[1].price) {
				askDepth.insert(askDepth.begin()+1, impliedAskDepth[0]);
			}

		}
	}
	void MarketDepth::fillImpliedBidDepth()
	{
		//暂时只对卖隐含价一进行处理
		if (impliedBidDepth.size() > 0 && bidDepth.size() > 0)
		{
			if (impliedBidDepth[0].price == bidDepth[0].price) {
				bidDepth[0].vol += impliedBidDepth[0].vol;
			}
			else if (impliedBidDepth[0].price < bidDepth[0].price) {
				bidDepth.insert(bidDepth.begin(), impliedBidDepth[0]);
			}
			else {
				if (impliedBidDepth.size() <= 1)
					return;
			}

			if (impliedBidDepth[0].price == bidDepth[1].price) {
				bidDepth[1].vol += impliedBidDepth[0].vol;
			}
			else if (impliedBidDepth[0].price < bidDepth[1].price) {
				bidDepth.insert(bidDepth.begin() + 1, impliedBidDepth[0]);
			}

		}
	}
	void MarketDepth::fillImpliedDepth()
	{
		filleImpliedAskDepth();
		fillImpliedBidDepth();
	}

}