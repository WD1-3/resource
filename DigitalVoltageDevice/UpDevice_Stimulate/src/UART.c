#include <reg52.h>
#include "UART.h"
typedef unsigned char u8;
u8 buffer[BUFFER_SIZE];
u8 bufferIndex = 0;
unsigned char frameReceived = 0;  // �������֡�Ƿ����

/**
 * ���ڳ�ʼ������
 */
void UsartInit()
{
    SCON = 0x50;     // ����Ϊ������ʽ1
    TMOD = 0x20;     // ���ü�����������ʽ2
    PCON = 0x80;     // �����ʼӱ�
    TH1 = 0xF3;      // ���ü�������ʼֵ (4800������)
    TL1 = 0xF3;
    ES = 1;          // �򿪽����ж�
    EA = 1;          // �����ж�
    TR1 = 1;         // �򿪼�����
}

/**
 * �����жϷ�����
 */
void Usart() interrupt 4
{
    static bit receiving = 0;  // �Ƿ����ڽ���֡
    u8 receiveData;

    if (RI)
    {
        RI = 0;  // ��������жϱ�־λ
        receiveData = SBUF;

        if (!receiving)
        {
            // ���֡ͷ
            if (receiveData == FRAME_HEAD1 && bufferIndex == 0)
            {
                buffer[bufferIndex++] = receiveData;
            }
            else if (receiveData == FRAME_HEAD2 && bufferIndex == 1)
            {
                buffer[bufferIndex++] = receiveData;
                receiving = 1;  // ��ʼ����֡����
            }
            else
            {
                bufferIndex = 0;  // ֡ͷ��ƥ�䣬���¿�ʼ����
            }
        }
        else
        {
            // ��������
            if (receiveData == FRAME_TAIL)
            {
                // ���յ�֡β�����֡�������
                buffer[bufferIndex++] = receiveData;
                frameReceived = 1;
                receiving = 0;
            }
            else
            {
                // �洢���ݵ�������
                if (bufferIndex < BUFFER_SIZE - 1)
                {
                    buffer[bufferIndex++] = receiveData;
                }
                else
                {
                    // ���������������֡
                    bufferIndex = 0;
                    receiving = 0;
                }
            }
        }
    }
}

/**
 * ����֡����
 * ������data[] - ��������, len - ���ݳ���
 */
void sendFrame(u8 *data__1, u8 len)
{
    u8 i;
    // ����֡ͷ
    SBUF = FRAME_HEAD1;
    while (!TI);
    TI = 0;

    SBUF = FRAME_HEAD2;
    while (!TI);
    TI = 0;

    // �������ݲ���
    for (i = 0; i < len; i++)
    {
        SBUF = data__1[i];
        while (!TI);
        TI = 0;
    }

    // ����֡β
    SBUF = FRAME_TAIL;
    while (!TI);
    TI = 0;
}

/**
 * ������
// */
//void main()
//{
//    u8 testData[] = {0x01, 0x02};  // ��������

//    UsartInit();  // ��ʼ������

//    while (1)
//    {
//			
////			  sendFrame(testData,2);
//        if (frameReceived)
//        {
//            frameReceived = 0;  // ������ձ��

//            // ������յ������ݣ����ݴ洢�� buffer[2] ~ buffer[bufferIndex-2]
//            if (bufferIndex > 3)  // ȷ��֡�������ٰ���֡ͷ��֡β��һ�������ֽ�
//            {
//							
//							
//							
//                sendFrame(&buffer[2], bufferIndex - 3);
//            }

//            // ���û���������
//            bufferIndex = 0;
//        }
//    }
//}



//        u8 dataLength = bufferIndex - 3; // ���ݲ��ֳ���
//        u8 i;

//        // �������ݲ���
//        for (i = 0; i < dataLength; i++)
//        {
//            u8 dataByte = buffer[2 + i];  // ���ݲ����� buffer[2] ~ buffer[bufferIndex - 2]
//            // ��������Ը�������������
//            P1 = dataByte;  // ���罫��������� P1
//        }
