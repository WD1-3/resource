#include "key.h"
#include "stdio.h"
#include <reg52.h>

/**************************************************************************************************** 
*                             长按、单击、双击定义
* 长按事件：任何大于 KEY_LONG_PRESS_TIME 
* 单击事件：按下时间不超过 KEY_LONG_PRESS_TIME 且 释放后 KEY_WAIT_DOUBLE_TIME 内无再次按下的操作
* 双击事件：俩次短按时间间隔小于KEY_WAIT_DOUBLE_TIME，俩次短按操作合并为一次双击事件。
* 特殊说明：
*          1.短按和长按时间间隔小于 KEY_WAIT_DOUBLE_TIME，响应一次单击和长按事件，不响应双击事件
*          2.连续2n次短按，且时间间隔小于 KEY_WAIT_DOUBLE_TIME，响应为n次双击
*          3.连续2n+1次短按，且时间间隔小于 KEY_WAIT_DOUBLE_TIME，且最后一次KEY_WAIT_DOUBLE_TIME内无操作，
*				响应为n次双击 和 一次单击事件
****************************************************************************************************/
#define KEY_LONG_PRESS_TIME    50 // 20ms*50 = 1s
#define KEY_WAIT_DOUBLE_TIME   25 // 20ms*25 = 500ms

#define KEY_PRESSED_LEVEL      0  // 按键按下是电平为低

sbit KEY_1 = P1^1;
sbit KEY_2 = P1^2;
/**************************************************************************************************** 
*                             局部函数定义
****************************************************************************************************/
static KEY_PinLevel_TypeDef KEY_ReadPin1(void);   // 按键读取按键的电平函数
static KEY_PinLevel_TypeDef KEY_ReadPin2(void);   // 按键读取按键的电平函数
static void KEY_GetAction_PressOrRelease(KEY_Configure_TypeDef *KeyCfg); // 获取按键是按下还是释放，保存到结构体

/**************************************************************************************************** 
*                             全局变量
****************************************************************************************************/
// KEY_Configure_TypeDef KeyCfg={		
// 		0,						//按键长按计数
// 		KEY_Action_Release,		//虚拟当前IO电平，按下1，抬起0
// 		KEY_Status_Idle,        //按键状态
// 		KEY_Event_Null,         //按键事件
// 		KEY_ReadPin             //读IO电平函数
// };

KEY_Configure_TypeDef KeyCfg1 = {		
    0,						
    KEY_Action_Release,		
    KEY_Status_Idle,        
    KEY_Event_Null,         
    KEY_ReadPin1            
};

KEY_Configure_TypeDef KeyCfg2 = {		
    0,						
    KEY_Action_Release,		
    KEY_Status_Idle,        
    KEY_Event_Null,         
    KEY_ReadPin2            
};
/**************************************************************************************************** 
*                             函数定义
****************************************************************************************************/
// 按键读取按键的电平函数，更具实际情况修改
static KEY_PinLevel_TypeDef KEY_ReadPin1(void)
{
    return (KEY_PinLevel_TypeDef)KEY_1;
}

static KEY_PinLevel_TypeDef KEY_ReadPin2(void)
{
    return (KEY_PinLevel_TypeDef)KEY_2;
}
// 获取按键是按下还是释放，保存到结构体
static void KEY_GetAction_PressOrRelease(KEY_Configure_TypeDef *KeyCfg)
{
    if(KeyCfg->KEY_ReadPin_Fcn() == KEY_PRESSED_LEVEL)
    {
        KeyCfg->KEY_Action = KEY_Action_Press;
    }
    else
    {
        KeyCfg->KEY_Action =  KEY_Action_Release;
    }
}


/**************************************************************************************************** 
*                             读取按键状态机
****************************************************************************************************/
void KEY_ReadStateMachine(KEY_Configure_TypeDef *KeyCfg)
{
    KEY_GetAction_PressOrRelease(KeyCfg);
    
    switch(KeyCfg->KEY_Status)
    {
        case KEY_Status_Idle:
            if(KeyCfg->KEY_Action == KEY_Action_Press)
            {
                KeyCfg->KEY_Status = KEY_Status_Debounce;
                KeyCfg->KEY_Event = KEY_Event_Null;
            }
            else
            {
                KeyCfg->KEY_Status = KEY_Status_Idle;
                KeyCfg->KEY_Event = KEY_Event_Null;
            }
            break;
            
        case KEY_Status_Debounce:
            if(KeyCfg->KEY_Action == KEY_Action_Press)
            {
                KeyCfg->KEY_Status = KEY_Status_ConfirmPress;
                KeyCfg->KEY_Event = KEY_Event_Null;
            }
            else
            {
                KeyCfg->KEY_Status = KEY_Status_Idle;
                KeyCfg->KEY_Event = KEY_Event_Null;
            }
            break;	

        case KEY_Status_ConfirmPress:
            if( (KeyCfg->KEY_Action == KEY_Action_Press) && ( KeyCfg->KEY_Count >= KEY_LONG_PRESS_TIME))
            {
                KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong;
                KeyCfg->KEY_Event = KEY_Event_Null;
                KeyCfg->KEY_Count = 0;
            }
            else if( (KeyCfg->KEY_Action == KEY_Action_Press) && (KeyCfg->KEY_Count < KEY_LONG_PRESS_TIME))
            {
                KeyCfg->KEY_Count++;
                KeyCfg->KEY_Status = KEY_Status_ConfirmPress;
                KeyCfg->KEY_Event = KEY_Event_Null;
            }
            else
            {
                KeyCfg->KEY_Count = 0;
                KeyCfg->KEY_Status = KEY_Status_WaiteAgain;
                KeyCfg->KEY_Event = KEY_Event_Null;
            }
            break;	
            
        case KEY_Status_ConfirmPressLong:
            if(KeyCfg->KEY_Action == KEY_Action_Press) 
            {
                KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong;
                KeyCfg->KEY_Event = KEY_Event_Null;
                KeyCfg->KEY_Count = 0;
            }
            else
            {
                KeyCfg->KEY_Status = KEY_Status_Idle;
                KeyCfg->KEY_Event = KEY_Event_LongPress;
                KeyCfg->KEY_Count = 0;
            }
            break;	
            
        case KEY_Status_WaiteAgain:
            if((KeyCfg->KEY_Action != KEY_Action_Press) && ( KeyCfg->KEY_Count >= KEY_WAIT_DOUBLE_TIME))
            {
                KeyCfg->KEY_Count = 0;
                KeyCfg->KEY_Status = KEY_Status_Idle;  
                KeyCfg->KEY_Event = KEY_Event_SingleClick;
            }
            else if((KeyCfg->KEY_Action != KEY_Action_Press) && ( KeyCfg->KEY_Count < KEY_WAIT_DOUBLE_TIME))
            {
                KeyCfg->KEY_Count ++;
                KeyCfg->KEY_Status = KEY_Status_WaiteAgain;
                KeyCfg->KEY_Event = KEY_Event_Null;
            }
            else
            {
                KeyCfg->KEY_Count = 0;
                KeyCfg->KEY_Status = KEY_Status_SecondPress;
                KeyCfg->KEY_Event = KEY_Event_Null;
            }
            break;		
        case KEY_Status_SecondPress:
            if( (KeyCfg->KEY_Action == KEY_Action_Press) && ( KeyCfg->KEY_Count >= KEY_LONG_PRESS_TIME))
            {
                KeyCfg->KEY_Status = KEY_Status_ConfirmPressLong;
                KeyCfg->KEY_Event = KEY_Event_SingleClick;
                KeyCfg->KEY_Count = 0;
            }
            else if( (KeyCfg->KEY_Action == KEY_Action_Press) && ( KeyCfg->KEY_Count < KEY_LONG_PRESS_TIME))
            {
                KeyCfg->KEY_Count ++;
                KeyCfg->KEY_Status = KEY_Status_SecondPress;
                KeyCfg->KEY_Event = KEY_Event_Null;
            }
            else 
            {
                KeyCfg->KEY_Count = 0;
                KeyCfg->KEY_Status = KEY_Status_Idle;
                KeyCfg->KEY_Event = KEY_Event_DoubleClick;
            }
            break;	
        default:
            break;
    }
}
