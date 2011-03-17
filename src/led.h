/*------------------------------------------------------------------------------------------------
** Program 	:	led.h
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/
#include "header.h"

#define LED_STANDBY 1
#define LED_WORK    2
#define LED_ERROR   3

#define LED_STANDBY_DELAY  0
#define LED_WORK_DELAY     2
#define LED_ERROR_DELAY    5

typedef struct _TLED
{
  uint16 count;
  uint16 channel;
  uint8 mode;
  uint8 toggle;
  void (*fnPhy)(uint16,uint8);
}TLED;

void LED_Init(void);
void LED_ModeSet(uint8 id,uint8 mode);

