#ifndef __I2C_H__
#define __I2C_H__
#define  PCF8591 0x90    //PCF8591 ��ַ

typedef unsigned char uchar; // ���� uchar ����

bit ISendByte(uchar sla,uchar c);
uchar IRcvByte(uchar sla);
unsigned int ADC_Read();

#endif