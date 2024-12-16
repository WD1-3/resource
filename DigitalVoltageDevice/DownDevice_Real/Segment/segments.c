#include <reg52.h>
#include "segments.h"

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
unsigned char code smgduan[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//显示0~9的值

//延时子函数
void Delay(unsigned int xms)
{
	unsigned char i, j;
	while(xms--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
}

//数码管显示
void DigDisplay(unsigned char addr, unsigned char number)
{
		switch(addr)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; P0=smgduan[number]|0x80;break;//显示第0位
			case(1):
				LSA=1;LSB=0;LSC=0; P0=smgduan[number];break;//显示第1位
			case(2):
				LSA=0;LSB=1;LSC=0; P0=smgduan[number]|0x80;break;//显示第2位
			case(3):
				LSA=1;LSB=1;LSC=0; P0=smgduan[number];break;//显示第3位
			case(4):
				LSA=0;LSB=0;LSC=1; P0=smgduan[number];break;//显示第4位
			case(5):
				LSA=1;LSB=0;LSC=1; P0=smgduan[number]|0x80;break;//显示第5位
			case(6):
				LSA=0;LSB=1;LSC=1; P0=smgduan[number];break;//显示第6位
			case(7):
				LSA=1;LSB=1;LSC=1; P0=smgduan[number];break;//显示第7位	
		}
//		P0=smgduan[number];//发送段码
		Delay(1); //间隔一段时间扫描	
		P0=0x00;//消隐
}



