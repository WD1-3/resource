#ifndef __BEEP_H__
#define __BEEP_H__	 

extern unsigned char H_limit; //上限初始值
extern unsigned char L_limit; //下限初始值
void beep_1(int limit);
void beep_2(int limit);
		    
#endif
//void beep_1(int limit) {
//	   int ad = 7;
//			 while(ad<7){
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
//				ad--;
//        // 这里可能需要一个退出条件
//			}
//}