#include<reg52.h>
#include"st7565.h"
#include "UART.h"
#include "beep.h"


//---声明一个全局变量---//
typedef unsigned char u8;
void Delay10ms(unsigned int c);
void Lcd_draw_point(uchar x,uchar y);
void uchar_to_string(unsigned char value, char* str_value, unsigned int size);
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
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
	char str_volt[4] = {0}; // 需要足够大的数组以容纳数字和结束符
	UsartInit();  
	Lcd12864_Init();
	Lcd12864_ClearScreen();
	Lcd12864_Write16CnCHAR(0, 6, "报警");
	Lcd12864_Write8CnCHAR(33	,6,"2.3" );
	Lcd12864_Write16CnCHAR(68, 6, "电压");


	while(1)
	{
	
				beep_3(ADC_Vallue[3]);
				
				for(k = 0;k<30;k++)
        if (frameReceived)
        {
				
            frameReceived = 0;  // 清除接收标记

            // 处理接收到的数据，数据存储在 buffer[2] ~ buffer[bufferIndex-2]
            if (bufferIndex > 3)  // 确保帧数据至少包含帧头、帧尾和一个数据字节
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
							uchar_to_string((unsigned char)y, str_volt,4); // 将 unsigned char 转换为字符串
							Lcd12864_Write8CnCHAR(98,6,str_volt);
							
							j++;
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

/*
void Delay10ms(unsigned int c)   //误差 0us
{
    unsigned char a,b;
    for(;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}

*/

void uchar_to_string(unsigned char value, char* str_value, unsigned int size) {
    if (size < 3) { // 检查空间是否足够
        str_value[0] = '\0'; // 赋值空字符串以避免意外
        return;
    }
    str_value[0] = (value / 10) + '0'; // 获取十位字符
    str_value[1] = '.'; // 获取个位字符
		str_value[2] = ((value) % 10) + '0'; // 获取个位字符
    str_value[3] = '\0'; // 添加结束符
}