#pragma once

#include <iostream>
#include <vector>

#include <sys/timeb.h>
namespace YWH_TOOLS{

	const char marketVersion = '1';

	//void parseMsgHeader(char flag, unsigned short onelen, unsigned short fiveLen, std::vector<char> &header);

	//数据类型
	enum MGS_SEND_TYPE {
		MSG_SEND_PRICE_DEPTH = 'A',//盘口数据
		MSG_SEND_HIDEEN_PRICE = 'B',
		MSG_SEND_HIGH_OPEN_LOW = 'C', //统计数据
		MSG_SEND_DEAL = 'D', //成交数据
		MSG_SEND_HEART_BEAT = 'E',//心跳数据
		MSG_SEND_CLEAR = 'F',
		MSG_SEND_INIT = 'I'//初始类型 
	};

	//成交数据class
	class MarketDealData {
	public:
		std::string exchange;			//交易所
		std::string contract;			//合约
		std::string dealPrice;			//成交价
		std::string dealVol;			//成交量
		std::string dateDealVol;	    //日成交量
		std::string bidAskFlag;			//主动买卖flag
		std::string HkDealFlag;		    //港股成交数据的flag,本程序可能不用了

		void clear() {
			exchange.clear();			//交易所
			contract.clear();			//合约
			dealPrice.clear();			//成交价
			dealVol.clear();			//成交量
			dateDealVol.clear();	    //日成交量
			bidAskFlag.clear();			//主动买卖flag
			HkDealFlag.clear();		    //港股成交数据的flag,可能不用了
		}
		std::string toString() {
			if (dealPrice == "")
				return "";
			std::string temp;
			temp.reserve(50);
			temp += exchange;
			temp += "@";
			temp += contract;
			temp += "@";
			temp += dealPrice;
			temp += "@";
			temp += dealVol;
			temp += "@";
			temp += bidAskFlag;
			temp += "@";
			temp += HkDealFlag;

		}
	};

	//统计数据class
	class MarketStatistics {
	public:
		std::string exchange;			//交易所
		std::string contract;			//合约
		std::string highPrice;			//最高价
		std::string lowPrice;			//最低价
		std::string openPrice;			//开盘价
		std::string closePriceForSettle;//收盘价
		std::string preSettementPrice;	//前结算价  
		std::string settementPrice;		//盘中结算价 SettlementTradePrice()?
		std::string hardenPrice;		//涨停价
		std::string dropPrice;			//跌停价
		std::string openInterest;		//持仓量

		bool operator==(MarketStatistics &other)const {
			return	this->highPrice == other.highPrice		&&
				this->lowPrice == other.lowPrice			&&
				this->openPrice == other.openPrice			&&
				this->closePriceForSettle == other.closePriceForSettle &&
				this->settementPrice == other.settementPrice	&&
				this->hardenPrice == other.hardenPrice		&&
				this->dropPrice == other.dropPrice			&&
				this->openInterest == other.openInterest;
		}

		void clear() {
			exchange.clear();			//交易所
			contract.clear();			//合约
			highPrice.clear();			//最高价
			lowPrice.clear();			//最低价
			openPrice.clear();			//开盘价
			closePriceForSettle.clear();//收盘价
			preSettementPrice.clear();	//前结算价  
			settementPrice.clear();		//盘中结算价 SettlementTradePrice()?
			hardenPrice.clear();		//涨停价
			dropPrice.clear();			//跌停价
			openInterest.clear();		//持仓量
		}

		std::string toString() {
			std::string temp;
			temp.reserve(50);
			temp += exchange;
			temp += "@";
			temp += contract;
			temp += "@";
			temp += highPrice;
			temp += "@";
			temp += lowPrice;
			temp += "@";
			temp += openPrice;
			temp += "@";
			temp += closePriceForSettle;
			temp += "@";
			temp += preSettementPrice;
			temp += "@";
			temp += settementPrice;
			temp += "@";
			temp += hardenPrice;
			temp += "@";
			temp += dropPrice;
			temp += "@";
			temp += openInterest;
		}

	};

	//行情发送结构
	struct SendMarketData
	{
		std::string data;
		int fiveLen;		//五档盘口长度
		int oneLen;			//一档盘口长度
		int tenLen;			//十档盘口长度
		bool useTcp;		//使用tcp发送 true
		MGS_SEND_TYPE msgType;


		SendMarketData() :
			fiveLen(0), oneLen(0), tenLen(0), useTcp(true), msgType(MGS_SEND_TYPE::MSG_SEND_INIT)
		{}

	};



	std::string DoubleToStringUsePoint(const double value, const int n)
	{
#define DOUBLE_MAX		10000000000
#define DOUBLE_MIN		-10000000000
		if (value > DOUBLE_MAX || value < DOUBLE_MIN)
			return "";
		char* temp[50] = { 0 };
		std::string qery = std::string("%.") + std::to_string(n) + "lf";//LF!=1F
		sprintf((char *)temp, qery.c_str(), value);
		std::string ret = std::string((char*)temp);
		if (ret == "0")
			return "";
		if (ret[0] == '0')
		{
			int flag = 0;
			if (ret.length() == 1)
			{

				return "";
			}

			for (int i = 2; i < 2 + n; i++)
			{
				if (ret[i] != '0')
				{
					flag = 1;
					break;
				}
			}
			if (0 == flag)
				ret = "";

		}

		return ret;
	}


	

	//int step;	价格乘数 10的次方的幂 
	inline int DoubleToStringByStep(const double val,int step)
	{
		if (step > 20 || step < 0)
			return 0;
		return  (int)(val*std::pow(10, step));
		
	}
	std::string DoubleToString(const double value, const int n)
	{
		if (value > DOUBLE_MAX || value < DOUBLE_MIN)
			return "";

		char* temp[50] = { 0 };
		std::string qery = std::string("%.") + std::to_string(n) + "lf";
		sprintf((char *)temp, qery.c_str(), value);
		std::string ret = std::string((char*)temp);
		if (ret == "0")
			return "";
		if (ret[0] == '0')
		{
			int flag = 0;
			if (ret.length() == 1)
			{

				return "";
			}

			for (int i = 2; i < 2 + n; i++)
			{
				if (ret[i] != '0')
				{
					flag = 1;
					break;
				}
			}
			if (0 == flag)
				ret = "";

		}
		int len = ret.length();
		if (ret.find(".") != std::string::npos)
		{
			std::remove(ret.begin(), ret.end(), '.');
			ret = ret.substr(0, ret.length() - 1);
		}

		return ret;


	}


}
