/*------------------------------------------------------------------------------------------------
** Program 	:	queue.c
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/
#include "header.h"

#include "queue.h"

//------------------------------------------------------------------------------------------------
//U8 Queue
void QueueU8Init(TQUEUEU8 *q)
{
  q->head=0;
  q->tail=0;
}

uint8 QueueU8IsFull(TQUEUEU8 *q)
{
  return q->head==((q->tail+1)%QUEUE_MAX);
}

uint8 QueueU8IsEmpty(TQUEUEU8 *q)
{
  return q->head==q->tail;
}

uint8 QueueU8Insert(TQUEUEU8 *q,uint8 data)
{
  if(!QueueU8IsFull(q))
  {
    q->data[q->tail]=data;
    q->tail=(q->tail+1)%QUEUE_MAX;
    return 1;
  }
  return 0;
}

uint8 QueueU8Delete(TQUEUEU8 *q,uint8 *data)
{
  if(!QueueU8IsEmpty(q))
  {
    *data=q->data[q->head];
    q->head=(q->head+1)%QUEUE_MAX;
    return 1;
  }
  return 0;
}
//End U8 Queue
//------------------------------------------------------------------------------------------------


