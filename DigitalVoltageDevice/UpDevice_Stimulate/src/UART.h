#ifndef __UART_H__
#define __UART_H__


#define FRAME_HEAD1  0xAA  // 帧头1
#define FRAME_HEAD2  0xAA  // 帧头2
#define FRAME_TAIL   0xFF  // 帧尾

// 数据缓存区
#define BUFFER_SIZE  32

//typedef unsigned char uchar;
void UsartInit();
void sendFrame(unsigned char *data__1, unsigned char len);


extern unsigned char buffer[BUFFER_SIZE];
extern unsigned char bufferIndex ;
extern unsigned char frameReceived;  // 标记完整帧是否接收

#endif