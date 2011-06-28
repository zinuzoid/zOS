/*------------------------------------------------------------------------------------------------
** Program 	:	main.c
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	16/3/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

#include "header.h"

#include "systick.h"
#include "led.h"
#include "usart.h"
#include "button.h"
#include "roundrobintask.h"
#include "cmd.h"
#include "util.h"

#include "stack.h"

/*
NVIC
5-USART
*/

int main()
{
  USARTAPI_Init();

  USART1_SendStr("\f\r\n=================USART1====================");
  USART2_SendStr("\f\r\n=================USART2====================");

  SysTick_Init();
  LED_Init();
  Button_Init();
  Cmd_Init();

  while(1)
  {
  //fast call
  
  //end fast call
    if(SysTick_1ms())
    {
      TaskRun();
    }
  }
}

void assert_failed(uint8_t* file, uint32_t line)
{
  while(1)
  {
    USART1_SendStr("assert_failed");
  }
}
