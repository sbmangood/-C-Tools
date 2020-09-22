#pragma once
#include <iostream>
#include <vector>

namespace YWH_TOOLS
{


	class MarketDepth
	{
	private:
		struct Depth {
			int price;		//价格
			int	vol;		//数量
			Depth()
				:vol(0), price(0)
			{
			}
			explicit Depth(int price_, int vol_)
				:price(price_), vol(vol_)
			{

			}
		};
	public:
		
		MarketDepth();
		void clear();
		bool pushAskDepth(double price, int vol) = delete;//可能不需要 用的是Depth
		bool pushAskDepth(int price,int vol);

		bool pushBidDepth(double price, int vol) = delete;
		bool pushBidDepth(int price, int vol);

		bool pushImpliedAskDepth(double price, int vol) = delete;
		bool pushImpliedAskDepth(int price, int vol);

		bool pushImpliedBidDepth(double price, int vol) = delete;
		bool pushImpliedBidDepth(int price, int vol);

	private:
		void filleImpliedAskDepth();
		void fillImpliedBidDepth();
		void fillImpliedDepth();

	public:

		std::string currentPrice;					//现价
		std::string currentVolume;					//现量
		std::string dateDealVol;					//日成交量 



		//扩展字段
		std::string marketLen;						//档位长度"1" "5" "10"目前有这三种
		int stepTick;								//跳点
		int step;									//价格乘数 10的次方的幂 
		std::string tradeTotalPrice;				//总成交额
		std::string msgSource;						//消息源
	private:

		std::vector<Depth> askDepth;				//买盘口
		std::vector<Depth> bidDepth;				//卖盘口
		std::vector<Depth> impliedAskDepth;			//隐含买盘口
		std::vector<Depth> impliedBidDepth;			//隐含卖盘口

		static const int depthLen = 10;				//最多行情档数
		static const int impliedLen = 2;			//最多隐含价个数
	};


}