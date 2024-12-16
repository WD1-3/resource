#ifndef __UART_H__
#define __UART_H__


#define FRAME_HEAD1  0xAA  // ֡ͷ1
#define FRAME_HEAD2  0xAA  // ֡ͷ2
#define FRAME_TAIL   0xFF  // ֡β

// ���ݻ�����
#define BUFFER_SIZE  32

//typedef unsigned char uchar;
void UsartInit();
void sendFrame(unsigned char *data__1, unsigned char len);


extern unsigned char buffer[BUFFER_SIZE];
extern unsigned char bufferIndex ;
extern unsigned char frameReceived;  // �������֡�Ƿ����

#endif