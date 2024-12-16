#include "stdio.h"
#include <reg52.h>
#include "segments.h"
sbit beep=P1^5;	
//void beep_1(int limit) {
//    while(1) {
//        if (limit > 5) {
//            beep = 1; // 打开蜂鸣器
//            Delay(1); // 延时大约1ms
//            beep = 0; // 关闭蜂鸣器
//            Delay(1);
//        } else if (limit < 2) {
//            beep = 1; 
//            Delay(6); 
//            beep = 0; 
//            Delay(6);
//        }
//        // 这里可能需要一个退出条件
//    }
//}
unsigned char H_limit = 45; //上限初始值
unsigned char L_limit = 10; //下限初始值


void beep_1(int limit) {
	   int ad = 100;
			 while(ad>0)
			{
        if (limit > H_limit) {
            beep = 1; // 打开蜂鸣器
            Delay(1); // 延时大约1ms
            beep = 0; // 关闭蜂鸣器
            Delay(1);
        } else if (limit < L_limit) {
            beep = 1; 
            Delay(1); 
            beep = 0; 
            Delay(1);
        }
				ad--;
        // 这里可能需要一个退出条件
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
        // 这里可能需要一个退出条件
			}
}