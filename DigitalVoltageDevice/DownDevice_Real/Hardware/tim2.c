#include <reg52.h>

void Init_timer2(void)
{
 RCAP2H=(65536-10000)/256;//赋T2初始值0x0bdc，溢出时间为50ms 现在才是10ms
 RCAP2L=(65536-10000)%256;
 TR2=1;	     //启动定时器2
 ET2=1;		 //打开定时器2中断
 EA=1;		 //打开总中断
}