#include <reg52.h>
#include "UART.h"
typedef unsigned char u8;
u8 buffer[BUFFER_SIZE];
u8 bufferIndex = 0;
unsigned char frameReceived = 0;  // 标记完整帧是否接收

/**
 * 串口初始化函数
 */
void UsartInit()
{
    SCON = 0x50;     // 设置为工作方式1
    TMOD = 0x20;     // 设置计数器工作方式2
    PCON = 0x80;     // 波特率加倍
    TH1 = 0xF3;      // 设置计数器初始值 (4800波特率)
    TL1 = 0xF3;
    ES = 1;          // 打开接收中断
    EA = 1;          // 打开总中断
    TR1 = 1;         // 打开计数器
}

/**
 * 串口中断服务函数
 */
void Usart() interrupt 4
{
    static bit receiving = 0;  // 是否正在接收帧
    u8 receiveData;

    if (RI)
    {
        RI = 0;  // 清除接收中断标志位
        receiveData = SBUF;

        if (!receiving)
        {
            // 检测帧头
            if (receiveData == FRAME_HEAD1 && bufferIndex == 0)
            {
                buffer[bufferIndex++] = receiveData;
            }
            else if (receiveData == FRAME_HEAD2 && bufferIndex == 1)
            {
                buffer[bufferIndex++] = receiveData;
                receiving = 1;  // 开始接收帧数据
            }
            else
            {
                bufferIndex = 0;  // 帧头不匹配，重新开始接收
            }
        }
        else
        {
            // 接收数据
            if (receiveData == FRAME_TAIL)
            {
                // 接收到帧尾，标记帧接收完成
                buffer[bufferIndex++] = receiveData;
                frameReceived = 1;
                receiving = 0;
            }
            else
            {
                // 存储数据到缓冲区
                if (bufferIndex < BUFFER_SIZE - 1)
                {
                    buffer[bufferIndex++] = receiveData;
                }
                else
                {
                    // 缓冲区溢出，丢弃帧
                    bufferIndex = 0;
                    receiving = 0;
                }
            }
        }
    }
}

/**
 * 发送帧数据
 * 参数：data[] - 数据数组, len - 数据长度
 */
void sendFrame(u8 *data__1, u8 len)
{
    u8 i;
    // 发送帧头
    SBUF = FRAME_HEAD1;
    while (!TI);
    TI = 0;

    SBUF = FRAME_HEAD2;
    while (!TI);
    TI = 0;

    // 发送数据部分
    for (i = 0; i < len; i++)
    {
        SBUF = data__1[i];
        while (!TI);
        TI = 0;
    }

    // 发送帧尾
    SBUF = FRAME_TAIL;
    while (!TI);
    TI = 0;
}

/**
 * 主函数
// */
//void main()
//{
//    u8 testData[] = {0x01, 0x02};  // 测试数据

//    UsartInit();  // 初始化串口

//    while (1)
//    {
//			
////			  sendFrame(testData,2);
//        if (frameReceived)
//        {
//            frameReceived = 0;  // 清除接收标记

//            // 处理接收到的数据，数据存储在 buffer[2] ~ buffer[bufferIndex-2]
//            if (bufferIndex > 3)  // 确保帧数据至少包含帧头、帧尾和一个数据字节
//            {
//							
//							
//							
//                sendFrame(&buffer[2], bufferIndex - 3);
//            }

//            // 重置缓冲区索引
//            bufferIndex = 0;
//        }
//    }
//}



//        u8 dataLength = bufferIndex - 3; // 数据部分长度
//        u8 i;

//        // 处理数据部分
//        for (i = 0; i < dataLength; i++)
//        {
//            u8 dataByte = buffer[2 + i];  // 数据部分在 buffer[2] ~ buffer[bufferIndex - 2]
//            // 在这里可以根据需求处理数据
//            P1 = dataByte;  // 例如将数据输出到 P1
//        }
