#include<reg52.h>
#include"st7565.h"
#include "UART.h"
#include "beep.h"


//---����һ��ȫ�ֱ���---//
typedef unsigned char u8;
void Delay10ms(unsigned int c);
void Lcd_draw_point(uchar x,uchar y);
void uchar_to_string(unsigned char value, char* str_value, unsigned int size);
/*******************************************************************************
* �� �� ��         : main
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : ��
*************************r******************************************************/

int ADC_Vallue[3];

void main()
{
	unsigned char i;
	unsigned char	j = 0;
	int y = 0;
	int b2 = 0;
	int b3 = 0;
	int VOLT = 0;
	unsigned char k = 0;
	unsigned char  h = 0;
	int draw_volt = 0;
	char str_volt[4] = {0}; // ��Ҫ�㹻����������������ֺͽ�����
	UsartInit();  
	Lcd12864_Init();
	Lcd12864_ClearScreen();
	Lcd12864_Write16CnCHAR(0, 6, "����");
	Lcd12864_Write8CnCHAR(33	,6,"2.3" );
	Lcd12864_Write16CnCHAR(68, 6, "��ѹ");


	while(1)
	{
	
				beep_3(ADC_Vallue[3]);
				
				for(k = 0;k<30;k++)
        if (frameReceived)
        {
				
            frameReceived = 0;  // ������ձ��

            // ������յ������ݣ����ݴ洢�� buffer[2] ~ buffer[bufferIndex-2]
            if (bufferIndex > 3)  // ȷ��֡�������ٰ���֡ͷ��֡β��һ�������ֽ�
            {
						  b2 = (int)(buffer[2]);
							b3 = (int)(buffer[3]);
							y =  (int)((b2 *100 +b3)/10);
							ADC_Vallue[1] = ADC_Vallue[0];
							ADC_Vallue[0] = y;
							ADC_Vallue[3] = (ADC_Vallue[0] - ADC_Vallue[1])*2;
							VOLT = y*0.9215;
							Lcd_draw_point(j,VOLT);
							if(y==51)
							{
								 y = 50;
							}
							uchar_to_string((unsigned char)y, str_volt,4); // �� unsigned char ת��Ϊ�ַ���
							Lcd12864_Write8CnCHAR(98,6,str_volt);
							
							j++;
            }

            // ���û���������
            bufferIndex = 0;
        }
				


	}
}

/*******************************************************************************
* �� �� ��         : Delay10ms
* ��������		   : ��ʱ��������ʱ10ms
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void Delay10ms(unsigned int c)   //��� 0us
{
    unsigned char a,b;
    for(;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}


void Lcd_draw_point(uchar x,uchar y)
{
		if (x > 128 || y > 64)
			return;
		LcdSt7565_WriteCmd(0xB0 + (y / 8)); //����Y���꣬������Կ��������������˵��
		LcdSt7565_WriteCmd(0x10 + ((x >> 4) & 0x0F));   //����X���꣬������Կ��������������˵��
		LcdSt7565_WriteCmd(0x00 + (x & 0x0F));
	  
	  LcdSt7565_WriteData(0x1 << (y % 8));  //������ñ���Ϊ��ɫʱ������ѡ��0XFF
}

/*
void Delay10ms(unsigned int c)   //��� 0us
{
    unsigned char a,b;
    for(;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}

*/

void uchar_to_string(unsigned char value, char* str_value, unsigned int size) {
    if (size < 3) { // ���ռ��Ƿ��㹻
        str_value[0] = '\0'; // ��ֵ���ַ����Ա�������
        return;
    }
    str_value[0] = (value / 10) + '0'; // ��ȡʮλ�ַ�
    str_value[1] = '.'; // ��ȡ��λ�ַ�
		str_value[2] = ((value) % 10) + '0'; // ��ȡ��λ�ַ�
    str_value[3] = '\0'; // ��ӽ�����
}