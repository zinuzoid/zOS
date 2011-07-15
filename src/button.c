/*------------------------------------------------------------------------------------------------
** Program 	:	button.c
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	16/7/2011
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
#define CHANNEL     1

typedef struct _TBUTTON
{
  uint8 channel;
  const char *channelname;
  uint8 count;
  uint8 logicout;
  uint8 state;
  uint8 toggle;
  uint16 presscount;
  uint8 (*fnPhy)(uint16);
}TBUTTON;

static void ButtonPHY_Init(void);
static uint8 ButtonPHY_getPORTA(uint16 channel);
static void ButtonTask_1ms(void *btn);
static void ButtonTask_Debouce1ms(void *btn);
static void ButtonTask_Init(uint8 taskchannel,const char *channelname,uint16 channel,uint8 *fn);
static void Button_PrintPolling(void* param);
static uint8 Button_Toggle(TBUTTON *btn,uint8 *btnlogic);
static uint8 Button_Latch(TBUTTON *btn);

TBUTTON tbtn[CHANNEL];

TTASK btntask[CHANNEL];
TTASK printtask[CHANNEL];


//------------------------------------------------------------------------------------------------
//Application Layer
void Button_Init(void)
{
  ButtonPHY_Init();
  ButtonTask_Init(0,"PA0",GPIO_Pin_0,(uint8*)ButtonPHY_getPORTA);
}

uint8 ButtonAPI_Logic(uint8 channel)
{
  if(channel<CHANNEL)
    return tbtn[channel].logicout;
  return 0;
}

uint8 ButtonAPI_Toggle(uint8 channel,uint8 *logic)
{
  if(channel<CHANNEL)
    return Button_Toggle(&tbtn[channel],logic);
  return 0;
}
//end Application Layer
//------------------------------------------------------------------------------------------------
//Middle Layer
static void Button_PrintPolling(void* param)
{
  TBUTTON *btn;
  uint8 logic,latch;

  if(!param)
    return;

  btn=param;

  if(Button_Toggle(&btn[0],&logic))
  {
    zprint("\r\nButton ");
    zprint((char*)btn->channelname);
    zprint(" Logic : ");
    logic+='0';
    zprint((char*)&logic);
  }
  
  latch=Button_Latch(btn);
  if(latch)
  {
    char tmp[2];
    tmp[0]=(char)(latch+'0');
    tmp[1]=0;
    zprint(tmp);
  }
}

static uint8 Button_Latch(TBUTTON *btn)
{
  return btn->presscount/1000;
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

static void ButtonTask_Init(uint8 taskchannel,const char *channelname,uint16 channel,uint8 *fn)
{
  static char buf1[20];
  static char buf2[20];
  
  if(taskchannel>=CHANNEL)
    return;
  if(!fn)
    return;
  
  TBUTTON *btn;
  btn=&tbtn[taskchannel];

  btn->channel=channel;
  btn->channelname=channelname;
  btn->count=0;
  btn->logicout=0;
  btn->state=0;
  btn->toggle=0;
  btn->presscount=0;
  btn->fnPhy=(uint8(*)(uint16))fn;

  sprintf(buf1,"Button%s",channelname);
  TaskAdd(&btntask[taskchannel],buf1,1,ButtonTask_1ms,btn);
  sprintf(buf2,"%sCheck",buf1);
  TaskAdd(&printtask[taskchannel],buf2,10,Button_PrintPolling,btn);
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
    else
    {
      btnptr->presscount=0;
      btnptr->state=10;
    }
    btnptr->logicout=1;
    break;
  case 10:
    if(btnptr->fnPhy(btnptr->channel))
    {
      btnptr->presscount++;
    }
    else
    {
      btnptr->presscount=0;
      btnptr->state=2;
    }
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

static uint8 ButtonPHY_getPORTA(uint16 channel)
{
  return GPIO_ReadInputDataBit(GPIOA,channel);
}
//end Physical Driver
//------------------------------------------------------------------------------------------------


