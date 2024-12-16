#include<reg51.h>
#include"st7565.h"
#include "UART.h"

//---����һ��ȫ�ֱ���---//
void Delay10ms(unsigned int c);
void Lcd_draw_point(uchar x,uchar y);
/*******************************************************************************
* �� �� ��         : main
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/



void main()
{
	unsigned char i;
//	unsigned char	j;
	UsartInit();
	Lcd12864_Init();
	Lcd12864_ClearScreen();
	
	for (i = 0; i < 128; ++i)
	{
		if ( i < 64)
			Lcd_draw_point(i,i);
		else
			Lcd_draw_point(i,128-i);
	}

	while(1)
	{
        if (frameReceived)
        {
            frameReceived = 0;  // ������ձ��

            // ������յ������ݣ����ݴ洢�� buffer[2] ~ buffer[bufferIndex-2]
            if (bufferIndex > 3)  // ȷ��֡�������ٰ���֡ͷ��֡β��һ�������ֽ�
            {
							
							
							
                sendFrame(&buffer[2], bufferIndex - 3);
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
