#include "stdio.h"
#include <reg52.h>
#include "segments.h"
sbit beep=P1^5;	
//void beep_1(int limit) {
//    while(1) {
//        if (limit > 5) {
//            beep = 1; // �򿪷�����
//            Delay(1); // ��ʱ��Լ1ms
//            beep = 0; // �رշ�����
//            Delay(1);
//        } else if (limit < 2) {
//            beep = 1; 
//            Delay(6); 
//            beep = 0; 
//            Delay(6);
//        }
//        // ���������Ҫһ���˳�����
//    }
//}
unsigned char H_limit = 45; //���޳�ʼֵ
unsigned char L_limit = 10; //���޳�ʼֵ


void beep_1(int limit) {
	   int ad = 100;
			 while(ad>0)
			{
        if (limit > H_limit) {
            beep = 1; // �򿪷�����
            Delay(1); // ��ʱ��Լ1ms
            beep = 0; // �رշ�����
            Delay(1);
        } else if (limit < L_limit) {
            beep = 1; 
            Delay(1); 
            beep = 0; 
            Delay(1);
        }
				ad--;
        // ���������Ҫһ���˳�����
			}
}
void beep_2(int limit) {
	   int ad = 49;
			 while(ad>0)
			{
        if (limit > H_limit) 
				{
            beep = 1; 
            Delay(6); 
            beep = 0; 
            Delay(6);
        }
				 else if (limit < L_limit) 
				{
            beep = 1; 
            Delay(6); 
            beep = 0; 
            Delay(6);
        }
				ad--;
        // ���������Ҫһ���˳�����
			}
}