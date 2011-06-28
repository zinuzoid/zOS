/*------------------------------------------------------------------------------------------------
** Program 	:	systick.c
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/
#include "header.h"

/*
static uint8 _5ms;
static uint8 _10ms;
static uint8 _100ms;
static uint8 _200ms;
static uint16 _500ms;
static uint16 _1000ms;
*/

static uint8 base1ms;
/*
static uint8 base5ms;
static uint8 base10ms;
static uint8 base100ms;
static uint8 base200ms;
static uint8 base500ms;
static uint8 base1000ms;
*/

void SysTick_Init(void)
{
  /*
  _5ms=0;
  _10ms=1;
  _100ms=2;
  _200ms=3;
  _500ms=4;
  _1000ms=5;
  */
	
  /*
  base5ms=0;
  base10ms=1;
  base100ms=2;
  base200ms=3;
  base500ms=4;
  base1000ms=5;
  */
  
  if(SysTick_Config(SystemCoreClock/1000))//ms
    while(1);
}

void SysTick_Handler(void)
{
  base1ms++;
  /*
  _5ms++;
  if(_5ms>=5)
  {
    _5ms=0;
    base5ms++;
  }
  
  _10ms++;
  if(_10ms>=10)
  {
    _10ms=0;
    base10ms++;
  }
  _100ms++;
  if(_100ms>=100)
  {
    _100ms=0;
    base100ms++;
  }
  
  _200ms++;
  if(_200ms>=200)
  {
    _200ms=0;
    base200ms++;
  }
  
  _500ms++;
  if(_500ms>=500)
  {
    _500ms=0;
    base500ms++;
  }
  
  _1000ms++;
  if(_1000ms>=1000)
  {
    _1000ms=0;
    base1000ms++;
  }
  */
}

uint8 SysTick_1ms(void)
{
  if(base1ms)
  {
    base1ms--;
    return 1;
  }
  return 0;
}
/*
uint8 SysTick_5ms(void)
{
  if(base5ms)
  {
    base5ms--;
    return 1;
  }
  return 0;
}

uint8 SysTick_10ms(void)
{
  if(base10ms)
  {
    base10ms--;
    return 1;
  }
  return 0;
}

uint8 SysTick_100ms(void)
{
  if(base100ms)
  {
    base100ms--;
    return 1;
  }
  return 0;
}

uint8 SysTick_200ms(void)
{
  if(base200ms)
  {
    base200ms--;
    return 1;
  }
  return 0;
}

uint8 SysTick_500ms(void)
{
  if(base500ms)
  {
    base500ms--;
    return 1;
  }
  return 0;
}

uint8 SysTick_1000ms(void)
{
  if(base1000ms)
  {
    base1000ms--;
    return 1;
  }
  return 0;
}
*/


