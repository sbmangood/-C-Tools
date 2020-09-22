#pragma once

#include <iostream>
#include <vector>

#include <sys/timeb.h>
namespace YWH_TOOLS{

	const char marketVersion = '1';

	//void parseMsgHeader(char flag, unsigned short onelen, unsigned short fiveLen, std::vector<char> &header);

	//��������
	enum MGS_SEND_TYPE {
		MSG_SEND_PRICE_DEPTH = 'A',//�̿�����
		MSG_SEND_HIDEEN_PRICE = 'B',
		MSG_SEND_HIGH_OPEN_LOW = 'C', //ͳ������
		MSG_SEND_DEAL = 'D', //�ɽ�����
		MSG_SEND_HEART_BEAT = 'E',//��������
		MSG_SEND_CLEAR = 'F',
		MSG_SEND_INIT = 'I'//��ʼ���� 
	};

	//�ɽ�����class
	class MarketDealData {
	public:
		std::string exchange;			//������
		std::string contract;			//��Լ
		std::string dealPrice;			//�ɽ���
		std::string dealVol;			//�ɽ���
		std::string dateDealVol;	    //�ճɽ���
		std::string bidAskFlag;			//��������flag
		std::string HkDealFlag;		    //�۹ɳɽ����ݵ�flag,��������ܲ�����

		void clear() {
			exchange.clear();			//������
			contract.clear();			//��Լ
			dealPrice.clear();			//�ɽ���
			dealVol.clear();			//�ɽ���
			dateDealVol.clear();	    //�ճɽ���
			bidAskFlag.clear();			//��������flag
			HkDealFlag.clear();		    //�۹ɳɽ����ݵ�flag,���ܲ�����
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

	//ͳ������class
	class MarketStatistics {
	public:
		std::string exchange;			//������
		std::string contract;			//��Լ
		std::string highPrice;			//��߼�
		std::string lowPrice;			//��ͼ�
		std::string openPrice;			//���̼�
		std::string closePriceForSettle;//���̼�
		std::string preSettementPrice;	//ǰ�����  
		std::string settementPrice;		//���н���� SettlementTradePrice()?
		std::string hardenPrice;		//��ͣ��
		std::string dropPrice;			//��ͣ��
		std::string openInterest;		//�ֲ���

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
			exchange.clear();			//������
			contract.clear();			//��Լ
			highPrice.clear();			//��߼�
			lowPrice.clear();			//��ͼ�
			openPrice.clear();			//���̼�
			closePriceForSettle.clear();//���̼�
			preSettementPrice.clear();	//ǰ�����  
			settementPrice.clear();		//���н���� SettlementTradePrice()?
			hardenPrice.clear();		//��ͣ��
			dropPrice.clear();			//��ͣ��
			openInterest.clear();		//�ֲ���
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

	//���鷢�ͽṹ
	struct SendMarketData
	{
		std::string data;
		int fiveLen;		//�嵵�̿ڳ���
		int oneLen;			//һ���̿ڳ���
		int tenLen;			//ʮ���̿ڳ���
		bool useTcp;		//ʹ��tcp���� true
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


	

	//int step;	�۸���� 10�Ĵη����� 
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
