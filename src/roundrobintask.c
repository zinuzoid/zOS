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

#include "roundrobintask.h"
#include "usart.h"
#include "util.h"

TTASK *taskheader;

void TaskRun(void)
{
  TTASK *taskptr;

  if(!taskheader)
    return;

  taskptr=taskheader;

  while(taskptr)
  {
    taskptr->count++;
    if(taskptr->count==taskptr->servicetime)
    {
      taskptr->count=0;
      taskptr->fn(taskptr->param);
    }

    taskptr=taskptr->next;
  }
}

void TaskAdd(TTASK *task,char *name,uint16 servicetime,void(*fn)(void*),void *param)
{
  TTASK *last;

  if(!task)
    return;

  task->name=name;
  task->servicetime=servicetime;
  task->fn=(void(*)(void*))fn;
  task->param=param;
  task->next=0;
  task->count=0;

  if(!taskheader)
  {
    taskheader=task;
    zprint("\r\nTask Header ");
    zprint(name);
    zprint(" Added.");
  }
  else
  {
    last=taskheader;
    while(last->next)
      last=last->next;
    last->next=task;

    zprint("\r\nTask ");
    zprint(name);
    zprint(" Added.");
  }
}

void TaskMonitor(TPRINT print)
{
  uint8 i=0;
  TTASK *taskrun;

  taskrun=taskheader;

  print("\r\n+-------------------------------------------------------------------+");
  print("\r\n+ Task Monitoring - RoundRobin Based                                +");
  print("\r\n+-------------------------------------------------------------------+");

  while(taskrun)
  {
    print("\r\n ");
    print(taskrun->name);
    taskrun=taskrun->next;
    i++;
  }
  print("\r\n+-------------------------------------------------------------------+");
}




