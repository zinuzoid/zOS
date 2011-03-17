/*------------------------------------------------------------------------------------------------
** Program 	:	led.c
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/
#include "header.h"

#include "led.h"
#include "roundrobintask.h"

TLED led[2];
TTASK taskled[2];

static void LEDPHY_Init(void);
static void LEDPHY_Set(uint16 channel,uint8 on_off);
static void LEDTask_Init(TLED *ledptr,uint16 channel,uint8 mode,void *fn);
static void LEDTask_100ms(void *param);

//------------------------------------------------------------------------------------------------
//Application Layer
void LED_Init(void)
{
  LEDPHY_Init();
  LEDTask_Init(&led[0],GPIO_Pin_8,LED_WORK,(void*)LEDPHY_Set);
  LEDTask_Init(&led[1],GPIO_Pin_9,LED_WORK,(void*)LEDPHY_Set);
  LED_ModeSet(0,LED_WORK);
  LED_ModeSet(1,LED_ERROR);

}

void LED_ModeSet(uint8 id,uint8 mode)
{
  led[id].mode=mode;
}
//end Application Layer
//------------------------------------------------------------------------------------------------
//Middle Layer
static void LEDTask_Init(TLED *ledptr,uint16 channel,uint8 mode,void *fn)
{
  if(!ledptr)
    return;

  ledptr->count=0;
  ledptr->mode=mode;
  ledptr->channel=channel;
  ledptr->toggle=0;
  ledptr->fnPhy=(void(*)(uint16,uint8))fn;

  switch(channel)
  {
  case GPIO_Pin_8:
      TaskAdd(&taskled[0],"LEDPC8",100,LEDTask_100ms,ledptr);
    break;
  case GPIO_Pin_9:
      TaskAdd(&taskled[1],"LEDPC9",100,LEDTask_100ms,ledptr);
    break;
  }
}

static void LEDTask_100ms(void *param)
{
  TLED *ledptr;

  if(!param)
    return;

  ledptr=param;

  switch(ledptr->mode)
  {
  case LED_WORK:
    ledptr->count++;
    if(ledptr->count==LED_WORK_DELAY)
    {
      ledptr->count=0;
      ledptr->fnPhy(ledptr->channel,1&ledptr->toggle);
      ledptr->toggle++;
    }
    break;
  case LED_ERROR:
    ledptr->count++;
    if(ledptr->count==LED_ERROR_DELAY)
    {
      ledptr->count=0;
      ledptr->fnPhy(ledptr->channel,1&ledptr->toggle);
      ledptr->toggle++;
    }
    break;
  case LED_STANDBY:
    if((1&ledptr->toggle)!=1)
    {
      ledptr->fnPhy(ledptr->channel,1&ledptr->toggle);
      ledptr->toggle++;
    }
  }
}
//end Middle Layer
//------------------------------------------------------------------------------------------------
//Physical Driver
static void LEDPHY_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIOInit;
	GPIOInit.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInit.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIOInit.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIOInit);
}

static void LEDPHY_Set(uint16 channel,uint8 on_off)
{
  GPIO_WriteBit(GPIOC,channel,(BitAction)on_off);
}

//end Physical Driver
//------------------------------------------------------------------------------------------------


