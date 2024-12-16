#ifndef __KEY_H__
#define __KEY_H__
#include "main.h"

#define ARR_LEN(arr) ((sizeof(arr)) / (sizeof(arr[0]))) // 数组大小宏函数
#define KEY_DEBOUNCE_TIME 7      // 消抖时间，单位：ms
#define KEY_LONG_PRESS_TIME 50   // 长按判定时间，单位：ms
#define KEY_DOUBLE_CLICK_TIME 30 // 双击判定时间，单位：ms
#define KEY_PRESSED_LEVEL 0       // 按键被按下时的电平

// 按键动作类型
typedef enum
{
  KEY_Action_Press,   // 按下
  KEY_Action_Release, // 松开
} KEY_Action_TypeDef;

// 按键状态类型
typedef enum
{
  KEY_Status_Idle,         // 空闲
  KEY_Status_Debounce,     // 消抖
  KEY_Status_Pressed,      // 按下
  KEY_Status_LongPress,    // 长按
  KEY_Status_WaitSecondPress, // 等待双击
} KEY_Status_TypeDef;

// 按键事件类型
typedef enum
{
  KEY_Event_Null,        // 无事件
  KEY_Event_SingleClick, // 单击
  KEY_Event_LongPress,   // 长按
  KEY_Event_DoubleClick, // 双击
  KEY_Event_Shake,       // 抖动
} KEY_Event_TypeDef;

// 按键配置结构体
typedef struct
{
  u8 KEY_Label;                   // 按键标号
  u16 KEY_Count;                  // 计时器计数值
  KEY_Action_TypeDef KEY_Action;  // 当前按键动作
  KEY_Status_TypeDef KEY_Status;  // 当前按键状态
  KEY_Event_TypeDef KEY_Event;    // 当前按键事件
} KEY_Configure_TypeDef;

extern KEY_Configure_TypeDef KeyConfig[2];
extern KEY_Event_TypeDef key_event[ARR_LEN(KeyConfig)];
void KEY_ReadStateMachine(KEY_Configure_TypeDef *KeyCfg);

#endif
