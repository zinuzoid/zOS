/*------------------------------------------------------------------------------------------------
** Program 	:	button.c
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

#include "header.h"

#include <stdio.h>

#include "button.h"
#include "usart.h"
#include "roundrobintask.h"

#define BOUNCE_TIME 40


static void ButtonPHY_Init(void);
static uint8 ButtonPHY_A0_Get(uint16 channel);
static void ButtonTask_1ms(void *btn);
static void ButtonTask_Debouce1ms(void *btn);
static void ButtonTask_Init(TBUTTON *btn,uint16 channel,const char *channelname,uint8 *fn);
static void Button_PrintPolling(void* param);
static uint8 Button_Toggle(TBUTTON *btn,uint8 *btnlogic);

TBUTTON btnpa0;

TTASK btntask;
TTASK printtask;


//------------------------------------------------------------------------------------------------
//Application Layer
void Button_Init(void)
{
  ButtonPHY_Init();
  ButtonTask_Init(&btnpa0,GPIO_Pin_0,"PA0",(uint8*)ButtonPHY_A0_Get);
}
uint8 Button_Get(TBUTTON *btn)
{
  return btn->logicout;
}
//end Application Layer
//------------------------------------------------------------------------------------------------
//Middle Layer

static void Button_PrintPolling(void* param)
{
  TBUTTON *btn;
  uint8 logic;

  if(!param)
    return;

  btn=param;

  if(Button_Toggle(&btnpa0,&logic))
  {
    zprint("\r\nButton ");
    zprint((char*)btn->channelname);
    zprint(" Logic : ");
    logic+='0';
    zprint((char*)&logic);
  }
}

static uint8 Button_Toggle(TBUTTON *btn,uint8 *btnlogic)
{
  if(!btn)
    return 0;

  if(btn->toggle!=btn->logicout)
  {
    btn->toggle=btn->logicout;
    *btnlogic=btn->logicout;
    return 1;
  }
  return 0;
}

static void ButtonTask_Init(TBUTTON *btn,uint16 channel,const char *channelname,uint8 *fn)
{
  static char buf1[20];
  static char buf2[20];

  if(!btn)
    return;
  if(!fn)
    return;

  btn->channel=channel;
  btn->channelname=channelname;
  btn->count=0;
  btn->logicout=0;
  btn->state=0;
  btn->toggle=0;
  btn->fnPhy=(uint8(*)(uint16))fn;

  sprintf(buf1,"Button%s",channelname);
  TaskAdd(&btntask,buf1,1,ButtonTask_1ms,btn);
  sprintf(buf2,"%sCheck",buf1);
  TaskAdd(&printtask,buf2,10,Button_PrintPolling,btn);
}

static void ButtonTask_1ms(void *btn)
{
  if(!btn)
    return;
  ButtonTask_Debouce1ms(btn);
}
static void ButtonTask_Debouce1ms(void *btn)
{
  TBUTTON *btnptr;

  if(!btn)
    return;

  btnptr=btn;

  if(!btnptr->fnPhy)
    return;

  switch(btnptr->state)
  {
  case 0:
    if(btnptr->fnPhy(btnptr->channel))
    {
      btnptr->state=1;
      btnptr->count=BOUNCE_TIME;
    }
    btnptr->logicout=0;
    break;
  case 1:
    if(btnptr->fnPhy(btnptr->channel))
    {
      if(!btnptr->count)
      {
        btnptr->state=2;
      }
      btnptr->count--;
    }
    else
    {
      btnptr->state=0;
    }
    btnptr->logicout=0;
    break;
  case 2:
    if(!btnptr->fnPhy(btnptr->channel))
    {
      btnptr->count=BOUNCE_TIME;
      btnptr->state=3;
    }
    btnptr->logicout=1;
    break;
  case 3:
    if(!btnptr->fnPhy(btnptr->channel))
    {
      if(!btnptr->count)
      {
        btnptr->state=0;
      }
      btnptr->count--;
    }
    else
    {
      btnptr->state=2;
    }
    btnptr->logicout=1;
  }
}
//end Middle Layer
//------------------------------------------------------------------------------------------------
//Physical Driver
static void ButtonPHY_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIOInit;
	GPIOInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIOInit.GPIO_Pin=GPIO_Pin_0;
	GPIOInit.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIOInit);
}

static uint8 ButtonPHY_A0_Get(uint16 channel)
{
  return GPIO_ReadInputDataBit(GPIOA,channel);
}
//end Physical Driver
//------------------------------------------------------------------------------------------------


