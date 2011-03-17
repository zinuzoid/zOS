/*------------------------------------------------------------------------------------------------
** Program 	:	roundrobintask.c
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

#include "header.h"
#include "cmd.h"

typedef struct _TTASK
{
  char *name;
  uint16 servicetime;
  void (*fn)(void*);
  void *param;
  uint16 count;
  struct _TTASK *next;
}TTASK;

void TaskRun(void);
void TaskAdd(TTASK *task,char *name,uint16 servicetime,void(*fn)(void*),void *param);
void TaskMonitor(TPRINT print);
