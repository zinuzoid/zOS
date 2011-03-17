/*------------------------------------------------------------------------------------------------
** Program 	:	button.h
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

typedef struct _TBUTTON
{
  uint8 channel;
  const char *channelname;
  uint8 count;
  uint8 logicout;
  uint8 state;
  uint8 toggle;
  uint8 (*fnPhy)(uint16);
}TBUTTON;

void Button_Init(void);
uint8 Button_Get(TBUTTON *btn);

