#pragma once
#include <iostream>
#include <vector>

namespace YWH_TOOLS
{


	class MarketDepth
	{
	private:
		struct Depth {
			int price;		//�۸�
			int	vol;		//����
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
		bool pushAskDepth(double price, int vol) = delete;//���ܲ���Ҫ �õ���Depth
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

		std::string currentPrice;					//�ּ�
		std::string currentVolume;					//����
		std::string dateDealVol;					//�ճɽ��� 



		//��չ�ֶ�
		std::string marketLen;						//��λ����"1" "5" "10"Ŀǰ��������
		int stepTick;								//����
		int step;									//�۸���� 10�Ĵη����� 
		std::string tradeTotalPrice;				//�ܳɽ���
		std::string msgSource;						//��ϢԴ
	private:

		std::vector<Depth> askDepth;				//���̿�
		std::vector<Depth> bidDepth;				//���̿�
		std::vector<Depth> impliedAskDepth;			//�������̿�
		std::vector<Depth> impliedBidDepth;			//�������̿�

		static const int depthLen = 10;				//������鵵��
		static const int impliedLen = 2;			//��������۸���
	};


}