#ifndef __I2C_H__
#define __I2C_H__
#define  PCF8591 0x90    //PCF8591 地址

typedef unsigned char uchar; // 定义 uchar 类型

bit ISendByte(uchar sla,uchar c);
uchar IRcvByte(uchar sla);
unsigned int ADC_Read();

#endif