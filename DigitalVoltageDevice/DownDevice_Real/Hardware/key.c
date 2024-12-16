#include "key.h"
#include "stdio.h"
#include <reg52.h>

/**************************************************************************************************** 
*                             ������������˫������
* �����¼����κδ��� KEY_LONG_PRESS_TIME 
* �����¼�������ʱ�䲻���� KEY_LONG_PRESS_TIME �� �ͷź� KEY_WAIT_DOUBLE_TIME �����ٴΰ��µĲ���
* ˫���¼������ζ̰�ʱ����С��KEY_WAIT_DOUBLE_TIME�����ζ̰������ϲ�Ϊһ��˫���¼���
* ����˵����
*          1.�̰��ͳ���ʱ����С�� KEY_WAIT_DOUBLE_TIME����Ӧһ�ε����ͳ����¼�������Ӧ˫���¼�
*          2.����2n�ζ̰�����ʱ����С�� KEY_WAIT_DOUBLE_TIME����ӦΪn��˫��
*          3.����2n+1�ζ̰�����ʱ����С�� KEY_WAIT_DOUBLE_TIME�������һ��KEY_WAIT_DOUBLE_TIME���޲�����
*				��ӦΪn��˫�� �� һ�ε����¼�
****************************************************************************************************/
#define KEY_LONG_PRESS_TIME    50 // 20ms*50 = 1s
#define KEY_WAIT_DOUBLE_TIME   25 // 20ms*25 = 500ms

#define KEY_PRESSED_LEVEL      0  // ���������ǵ�ƽΪ��

sbit KEY_1 = P1^1;
sbit KEY_2 = P1^2;
/**************************************************************************************************** 
*                             �ֲ���������
****************************************************************************************************/
static KEY_PinLevel_TypeDef KEY_ReadPin1(void);   // ������ȡ�����ĵ�ƽ����
static KEY_PinLevel_TypeDef KEY_ReadPin2(void);   // ������ȡ�����ĵ�ƽ����
static void KEY_GetAction_PressOrRelease(KEY_Configure_TypeDef *KeyCfg); // ��ȡ�����ǰ��»����ͷţ����浽�ṹ��

/**************************************************************************************************** 
*                             ȫ�ֱ���
****************************************************************************************************/
// KEY_Configure_TypeDef KeyCfg={		
// 		0,						//������������
// 		KEY_Action_Release,		//���⵱ǰIO��ƽ������1��̧��0
// 		KEY_Status_Idle,        //����״̬
// 		KEY_Event_Null,         //�����¼�
// 		KEY_ReadPin             //��IO��ƽ����
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
*                             ��������
****************************************************************************************************/
// ������ȡ�����ĵ�ƽ����������ʵ������޸�
static KEY_PinLevel_TypeDef KEY_ReadPin1(void)
{
    return (KEY_PinLevel_TypeDef)KEY_1;
}

static KEY_PinLevel_TypeDef KEY_ReadPin2(void)
{
    return (KEY_PinLevel_TypeDef)KEY_2;
}
// ��ȡ�����ǰ��»����ͷţ����浽�ṹ��
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
*                             ��ȡ����״̬��
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
