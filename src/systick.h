/*------------------------------------------------------------------------------------------------
** Program 	:	systick.h
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

#ifndef _SYSTICK_H
#define _SYSTICK_H

void SysTick_Handler(void);

void SysTick_Init(void);

u8 SysTick_1ms(void);
/*
u8 SysTick_5ms(void);
u8 SysTick_10ms(void);
u8 SysTick_100ms(void);
u8 SysTick_200ms(void);
u8 SysTick_500ms(void);
u8 SysTick_1000ms(void);
*/

#endif
