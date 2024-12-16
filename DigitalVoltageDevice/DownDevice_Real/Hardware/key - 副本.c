#include "key.h"
#include <reg52.h>
sbit KEY_1 = P1^0;
sbit KEY_2 = P1^1;
// ������ȡ����
static u8 KEY_ReadPin(u8 key_label)
{
  switch (key_label)
  {
  case 0:
    return  (u8)KEY_1;
	  break;
  case 1:
    return  (u8)KEY_2;	  
	  break;
  default:
    return 1; // Ĭ�ϸߵ�ƽ��δ���£�	  
	  break;
  }
}

// ������������
KEY_Configure_TypeDef KeyConfig[2] = {
    {0, 0, KEY_Action_Release, KEY_Status_Idle, KEY_Event_Null},
    {1, 0, KEY_Action_Release, KEY_Status_Idle, KEY_Event_Null},
};

KEY_Event_TypeDef key_event[2] = {KEY_Event_Null,KEY_Event_Null}; // �����¼�����

// ����״̬��
void KEY_ReadStateMachine(KEY_Configure_TypeDef *KeyCfg)
{
  // ��ȡ��������
  if (KEY_ReadPin(KeyCfg->KEY_Label) == KEY_PRESSED_LEVEL)
    KeyCfg->KEY_Action = KEY_Action_Press;
  else
    KeyCfg->KEY_Action = KEY_Action_Release;

  // ״̬������
  switch (KeyCfg->KEY_Status)
  {
  case KEY_Status_Idle: // ����״̬
    if (KeyCfg->KEY_Action == KEY_Action_Press) // ��⵽����
    {
      KeyCfg->KEY_Status = KEY_Status_Debounce;
      KeyCfg->KEY_Count = 0;
    }
    break;

  case KEY_Status_Debounce: // ����״̬
    if (KeyCfg->KEY_Action == KEY_Action_Press)
    {
      if (++KeyCfg->KEY_Count >= KEY_DEBOUNCE_TIME)
      {
        KeyCfg->KEY_Status = KEY_Status_Pressed; // ������ɣ����밴��״̬
        KeyCfg->KEY_Event = KEY_Event_Null;
      }
    }
    else
    {
      if (KeyCfg->KEY_Count > 0) // ����⵽���µ��ͷ�
        KeyCfg->KEY_Event = KEY_Event_Shake; // ��¼�����¼�
      KeyCfg->KEY_Status = KEY_Status_Idle; // ���ؿ���״̬
      KeyCfg->KEY_Count = 0;
    }
    break;

  case KEY_Status_Pressed: // ����״̬
    if (KeyCfg->KEY_Action == KEY_Action_Release) // �ɿ�����
    {
      if (KeyCfg->KEY_Count < KEY_DOUBLE_CLICK_TIME)
        KeyCfg->KEY_Status = KEY_Status_WaitSecondPress; // �ȴ�˫��
      else
      {
        KeyCfg->KEY_Status = KEY_Status_Idle;
        KeyCfg->KEY_Event = KEY_Event_SingleClick; // �����¼�
      }
			KeyCfg->KEY_Count = 0; // �����ʱ��
    }
    else if (++KeyCfg->KEY_Count >= KEY_LONG_PRESS_TIME) // ����ʱ��ﵽ������ֵ
    {
      KeyCfg->KEY_Status = KEY_Status_LongPress;
      KeyCfg->KEY_Event = KEY_Event_LongPress; // ���������¼�
    }
    break;

  case KEY_Status_LongPress: // ����״̬
    if (KeyCfg->KEY_Action == KEY_Action_Release) // �ɿ�����
    {
      KeyCfg->KEY_Status = KEY_Status_Idle;
			KeyCfg->KEY_Event = KEY_Event_Null;   // ����¼�
    }
    break;

  case KEY_Status_WaitSecondPress: // �ȴ�˫��״̬
    if (++KeyCfg->KEY_Count >= KEY_DOUBLE_CLICK_TIME) // ��ʱδ����
    {
      KeyCfg->KEY_Status = KEY_Status_Idle;
      KeyCfg->KEY_Event = KEY_Event_SingleClick; // ��ʱ���������¼�
    }
    else if (KeyCfg->KEY_Action == KEY_Action_Press) // ��⵽�ڶ��ΰ���
    {
      KeyCfg->KEY_Status = KEY_Status_Debounce; // �ٴ�����
      KeyCfg->KEY_Event = KEY_Event_DoubleClick; // ����˫���¼�
    }
    break;
  }

  // �����¼���¼
  if (KeyCfg->KEY_Event != KEY_Event_Null)
    key_event[KeyCfg->KEY_Label] = KeyCfg->KEY_Event;
}
