#include "stdio.h"
#include <reg52.h>
sbit beep=P1^5;

void Delay(unsigned int xms);
void beep_3(int limit) {
	   int ad = 49;
			 while(ad>0)
			{
				if (limit<0)limit  = -limit;
        if (limit > 50) 
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