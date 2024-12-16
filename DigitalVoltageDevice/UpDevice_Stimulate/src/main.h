#include<reg51.h>
#include"st7565.h"
#include "UART.h"

//---声明一个全局变量---//
void Delay10ms(unsigned int c);
void Lcd_draw_point(uchar x,uchar y);
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
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
            frameReceived = 0;  // 清除接收标记

            // 处理接收到的数据，数据存储在 buffer[2] ~ buffer[bufferIndex-2]
            if (bufferIndex > 3)  // 确保帧数据至少包含帧头、帧尾和一个数据字节
            {
							
							
							
                sendFrame(&buffer[2], bufferIndex - 3);
            }

            // 重置缓冲区索引
            bufferIndex = 0;
        }

	}
}

/*******************************************************************************
* 函 数 名         : Delay10ms
* 函数功能		   : 延时函数，延时10ms
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Delay10ms(unsigned int c)   //误差 0us
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
		LcdSt7565_WriteCmd(0xB0 + (y / 8)); //设置Y坐标，具体可以看清屏函数里面的说明
		LcdSt7565_WriteCmd(0x10 + ((x >> 4) & 0x0F));   //设置X坐标，具体可以看清屏函数里面的说明
		LcdSt7565_WriteCmd(0x00 + (x & 0x0F));
	  
	  LcdSt7565_WriteData(0x1 << (y % 8));  //如果设置背景为白色时，清屏选择0XFF
}
