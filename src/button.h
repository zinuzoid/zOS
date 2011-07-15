/*------------------------------------------------------------------------------------------------
** Program 	:	button.h
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	16/7/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

#ifndef _BUTTON_H
#define _BUTTON_H

void Button_Init(void);
uint8 ButtonAPI_Logic(uint8 channel);
uint8 ButtonAPI_Toggle(uint8 channel,uint8 *logic);

#endif




