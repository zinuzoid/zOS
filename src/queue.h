/*------------------------------------------------------------------------------------------------
** Program 	:	queue.h
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/
#include "header.h"

#ifndef _QUEUE_H
#define _QUEUE_H

#define QUEUE_MAX 1024

typedef struct _TQUEUE
{
  uint8 data[QUEUE_MAX];
  uint16 head;//uint8 only for QUEUE_MAX 256
  uint16 tail;
}TQUEUEU8;


void QueueU8Init(TQUEUEU8 *q);
uint8 QueueU8IsFull(TQUEUEU8 *q);
uint8 QueueU8IsEmpty(TQUEUEU8 *q);
uint8 QueueU8Insert(TQUEUEU8 *q,uint8 data);
uint8 QueueU8Delete(TQUEUEU8 *q,uint8 *data);

#endif

