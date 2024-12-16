//����˵��:
//SCL---P20
//SDA---P21
//�����޸Ľ���,����I2C.c�ļ����޸�
#include <reg52.h>
#include "I2C.H"
#include "segments.h"
#include "tim2.h"
#include "main.h"
#include "key.h"
#include "beep.h"
#include "UART.h"


/*
typedef enum
{
  KEY_Event_Null,        // ���¼�
  KEY_Event_SingleClick, // ����
  KEY_Event_LongPress,   // ����
  KEY_Event_DoubleClick, // ˫��
  KEY_Event_Shake,       // ����
} KEY_Event_TypeDef;
*/
//void UART_SendNumber(int num);

unsigned long xdata LedOut[4]; //��ѹ�������ʾֵ
unsigned long xdata UedOut[4]; //��ѹ����������
unsigned int  ADC_Vallue = 0;//��ѹԭʼֵ
char  ADC_DATA[2]; //��ѹ����ֵ
unsigned int  ADC_Warning = 0; //��ѹ�������
//u8 H_limit = 45; //���޳�ʼֵ
//u8 L_limit = 10; //���޳�ʼֵ
void main(void)
{ 
 P0 = 0x00;  // ��ն��룬�ر���ʾ
 P1 |= 0x03; //��ʼ��IO��Ϊ�ߵ�ƽ
 Init_timer2();
 UsartInit();
 while(1)
 {/********����AD-DA����*************/

	 ADC_Vallue = ADC_Read();
	 UedOut[0] = H_limit/10;
	 UedOut[1] = H_limit%10;
	 UedOut[2] = L_limit/10;
	 UedOut[3] = L_limit%10;
	 DigDisplay(0,UedOut[0]);
	 DigDisplay(1,UedOut[1]);
	 DigDisplay(2,UedOut[2]);
	 DigDisplay(3,UedOut[3]);
	 
	 DigDisplay(4,LedOut[0]);
	 DigDisplay(5,LedOut[1]);
	 DigDisplay(6,LedOut[2]);
	 DigDisplay(7,LedOut[3]);
	 
	 
//	 UART_SendNumber(ADC_Vallue); //0

 }  
}

//��ʱ���жϺ���
void Timer2() interrupt 5	  //��ʱ��2��5���ж�
{    
	   
	   static u8 count = 1; //��ѹ��������
		 static u8 count1 = 1; //beep��������
	   
     TF2=0;
        KEY_ReadStateMachine(&KeyCfg1);
        KEY_ReadStateMachine(&KeyCfg2);
			

		         // ���ݰ����¼�ִ����Ӧ����
			if (KeyCfg1.KEY_Event == KEY_Event_SingleClick) {
//									sendFrame(&H_limit,1);
									H_limit +=3;


			} else if (KeyCfg1.KEY_Event == KEY_Event_DoubleClick) {
//								sendFrame(&L_limit,1);
								H_limit-= 3;
								
					// ������1˫���¼�
			} else if (KeyCfg1.KEY_Event == KEY_Event_LongPress) {
									H_limit = 30;
//									sendFrame(&H_limit,1);
									
			}

			if (KeyCfg2.KEY_Event == KEY_Event_SingleClick) {
									L_limit +=3;
			} else if (KeyCfg2.KEY_Event == KEY_Event_DoubleClick) {
					        L_limit-= 3;
			} else if (KeyCfg2.KEY_Event == KEY_Event_LongPress) {
					        L_limit = 20;
			}
		
	   if(count== 51 |count>51)
		 {
			 ADC_DATA[0] = ADC_Vallue/100;
			 ADC_DATA[1] = ADC_Vallue%100;
			 LedOut[0]=ADC_Vallue%10000/1000;
			 LedOut[1]=ADC_Vallue%1000/100;
			 LedOut[2]=ADC_Vallue%100/10;
			 LedOut[3]=ADC_Vallue%10;
       ADC_Warning  = LedOut[1] *10 + LedOut[2];
			 sendFrame(ADC_DATA, 2);
		   count = 0;
		 }
		 	if(count1== 200 |count1>200)
		 {
			 beep_1(ADC_Warning);
		   beep_2(ADC_Warning);			 
		   count1 = 0;
		 }
		 count++;
		 count1++;
}







