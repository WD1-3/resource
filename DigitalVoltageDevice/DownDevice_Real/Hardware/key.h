#ifndef __KEY_H
#define __KEY_H	 
#include "main.h"

// ����״̬
typedef enum _KEY_StatusList_TypeDef 
{
	KEY_Status_Idle = 0				, // ����
	KEY_Status_Debounce   		    , // ����
	KEY_Status_ConfirmPress		    , // ȷ�ϰ���	
	KEY_Status_ConfirmPressLong		, // ȷ�ϳ�����	
	KEY_Status_WaiteAgain		    , // �ȴ��ٴΰ���
	KEY_Status_SecondPress          , // �ڶ��ΰ���
}KEY_StatusList_TypeDef;

// �����¼�
typedef enum _KEY_EventList_TypeDef 
{
	KEY_Event_Null 		   = 0x00,  // ���¼�
	KEY_Event_SingleClick  = 0x01,  // ����
	KEY_Event_DoubleClick  = 0x02,  // ˫��
	KEY_Event_LongPress    = 0x04   // ����
}KEY_EventList_TypeDef;

// �������������¡��ͷ�
typedef enum
{ 
	KEY_Action_Press = 0,
	KEY_Action_Release
}KEY_Action_TypeDef;

// �������ŵĵ�ƽ
typedef enum
{ 
	KKEY_PinLevel_Low = 0,
	KEY_PinLevel_High
}KEY_PinLevel_TypeDef;

// �������ýṹ��
typedef struct _KEY_Configure_TypeDef 
{
	u16                        KEY_Count;        //������������
	KEY_Action_TypeDef             KEY_Action;        //��������������1��̧��0
	KEY_StatusList_TypeDef         KEY_Status;        //����״̬
	KEY_EventList_TypeDef          KEY_Event;          //�����¼�
	KEY_PinLevel_TypeDef          (*KEY_ReadPin_Fcn)(void);    //��IO��ƽ����
}KEY_Configure_TypeDef;

extern KEY_Configure_TypeDef KeyCfg1;
extern KEY_Configure_TypeDef KeyCfg2;
// KEY_Configure_TypeDef KeyCfg2
//������ʼ������
void KEY_Init(void); //IO��ʼ��
void KEY_ReadStateMachine(KEY_Configure_TypeDef *KeyCfg);


			    
#endif
