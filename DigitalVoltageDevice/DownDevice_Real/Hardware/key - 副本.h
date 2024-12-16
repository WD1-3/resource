#ifndef __KEY_H__
#define __KEY_H__
#include "main.h"

#define ARR_LEN(arr) ((sizeof(arr)) / (sizeof(arr[0]))) // �����С�꺯��
#define KEY_DEBOUNCE_TIME 7      // ����ʱ�䣬��λ��ms
#define KEY_LONG_PRESS_TIME 50   // �����ж�ʱ�䣬��λ��ms
#define KEY_DOUBLE_CLICK_TIME 30 // ˫���ж�ʱ�䣬��λ��ms
#define KEY_PRESSED_LEVEL 0       // ����������ʱ�ĵ�ƽ

// ������������
typedef enum
{
  KEY_Action_Press,   // ����
  KEY_Action_Release, // �ɿ�
} KEY_Action_TypeDef;

// ����״̬����
typedef enum
{
  KEY_Status_Idle,         // ����
  KEY_Status_Debounce,     // ����
  KEY_Status_Pressed,      // ����
  KEY_Status_LongPress,    // ����
  KEY_Status_WaitSecondPress, // �ȴ�˫��
} KEY_Status_TypeDef;

// �����¼�����
typedef enum
{
  KEY_Event_Null,        // ���¼�
  KEY_Event_SingleClick, // ����
  KEY_Event_LongPress,   // ����
  KEY_Event_DoubleClick, // ˫��
  KEY_Event_Shake,       // ����
} KEY_Event_TypeDef;

// �������ýṹ��
typedef struct
{
  u8 KEY_Label;                   // �������
  u16 KEY_Count;                  // ��ʱ������ֵ
  KEY_Action_TypeDef KEY_Action;  // ��ǰ��������
  KEY_Status_TypeDef KEY_Status;  // ��ǰ����״̬
  KEY_Event_TypeDef KEY_Event;    // ��ǰ�����¼�
} KEY_Configure_TypeDef;

extern KEY_Configure_TypeDef KeyConfig[2];
extern KEY_Event_TypeDef key_event[ARR_LEN(KeyConfig)];
void KEY_ReadStateMachine(KEY_Configure_TypeDef *KeyCfg);

#endif
