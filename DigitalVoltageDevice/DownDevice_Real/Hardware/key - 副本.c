#include "key.h"
#include <reg52.h>
sbit KEY_1 = P1^0;
sbit KEY_2 = P1^1;
// 按键读取函数
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
    return 1; // 默认高电平（未按下）	  
	  break;
  }
}

// 按键配置数组
KEY_Configure_TypeDef KeyConfig[2] = {
    {0, 0, KEY_Action_Release, KEY_Status_Idle, KEY_Event_Null},
    {1, 0, KEY_Action_Release, KEY_Status_Idle, KEY_Event_Null},
};

KEY_Event_TypeDef key_event[2] = {KEY_Event_Null,KEY_Event_Null}; // 按键事件数组

// 按键状态机
void KEY_ReadStateMachine(KEY_Configure_TypeDef *KeyCfg)
{
  // 读取按键动作
  if (KEY_ReadPin(KeyCfg->KEY_Label) == KEY_PRESSED_LEVEL)
    KeyCfg->KEY_Action = KEY_Action_Press;
  else
    KeyCfg->KEY_Action = KEY_Action_Release;

  // 状态机处理
  switch (KeyCfg->KEY_Status)
  {
  case KEY_Status_Idle: // 空闲状态
    if (KeyCfg->KEY_Action == KEY_Action_Press) // 检测到按下
    {
      KeyCfg->KEY_Status = KEY_Status_Debounce;
      KeyCfg->KEY_Count = 0;
    }
    break;

  case KEY_Status_Debounce: // 消抖状态
    if (KeyCfg->KEY_Action == KEY_Action_Press)
    {
      if (++KeyCfg->KEY_Count >= KEY_DEBOUNCE_TIME)
      {
        KeyCfg->KEY_Status = KEY_Status_Pressed; // 消抖完成，进入按下状态
        KeyCfg->KEY_Event = KEY_Event_Null;
      }
    }
    else
    {
      if (KeyCfg->KEY_Count > 0) // 曾检测到按下但释放
        KeyCfg->KEY_Event = KEY_Event_Shake; // 记录抖动事件
      KeyCfg->KEY_Status = KEY_Status_Idle; // 返回空闲状态
      KeyCfg->KEY_Count = 0;
    }
    break;

  case KEY_Status_Pressed: // 按下状态
    if (KeyCfg->KEY_Action == KEY_Action_Release) // 松开按键
    {
      if (KeyCfg->KEY_Count < KEY_DOUBLE_CLICK_TIME)
        KeyCfg->KEY_Status = KEY_Status_WaitSecondPress; // 等待双击
      else
      {
        KeyCfg->KEY_Status = KEY_Status_Idle;
        KeyCfg->KEY_Event = KEY_Event_SingleClick; // 单击事件
      }
			KeyCfg->KEY_Count = 0; // 清零计时器
    }
    else if (++KeyCfg->KEY_Count >= KEY_LONG_PRESS_TIME) // 按下时间达到长按阈值
    {
      KeyCfg->KEY_Status = KEY_Status_LongPress;
      KeyCfg->KEY_Event = KEY_Event_LongPress; // 触发长按事件
    }
    break;

  case KEY_Status_LongPress: // 长按状态
    if (KeyCfg->KEY_Action == KEY_Action_Release) // 松开按键
    {
      KeyCfg->KEY_Status = KEY_Status_Idle;
			KeyCfg->KEY_Event = KEY_Event_Null;   // 清空事件
    }
    break;

  case KEY_Status_WaitSecondPress: // 等待双击状态
    if (++KeyCfg->KEY_Count >= KEY_DOUBLE_CLICK_TIME) // 超时未按下
    {
      KeyCfg->KEY_Status = KEY_Status_Idle;
      KeyCfg->KEY_Event = KEY_Event_SingleClick; // 超时触发单击事件
    }
    else if (KeyCfg->KEY_Action == KEY_Action_Press) // 检测到第二次按下
    {
      KeyCfg->KEY_Status = KEY_Status_Debounce; // 再次消抖
      KeyCfg->KEY_Event = KEY_Event_DoubleClick; // 触发双击事件
    }
    break;
  }

  // 更新事件记录
  if (KeyCfg->KEY_Event != KEY_Event_Null)
    key_event[KeyCfg->KEY_Label] = KeyCfg->KEY_Event;
}
