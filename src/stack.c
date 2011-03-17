/*------------------------------------------------------------------------------------------------
** Program 	:	stack.c
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	17/3/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.>
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

#include "header.h"

#include "stack.h"
#include "stm32f10x_it.h"//debug

//------------------------------------------------------------------------------------------------
//U8 Stack
void StackU8Init(TSTACKU8 *s)
{
	s->top=STACK_EMPTY;
}

uint8 StackU8IsEmpty(TSTACKU8 *s)
{
	return s->top==STACK_EMPTY;
}

uint8 StackU8IsFull(TSTACKU8 *s)
{
  return s->top==STACK_SIZE_U8-1;
}

uint8 StackU8Push(TSTACKU8 *s,uint8 value)
{
  if(!StackU8IsFull(s))
  {
    s->top++;
    s->value[s->top]=value;
    return 1;
  }
  return 0;
}

uint8 StackU8Pop(TSTACKU8 *s,uint8 *value)
{
	if(!StackU8IsEmpty(s))
	{
		*value=s->value[s->top];
		s->top--;
		return 1;
	}
  UsageFault_Handler();//debug
  return 0;
}
//End U8 Stack
//------------------------------------------------------------------------------------------------

#ifdef _STACKFLOAT
//------------------------------------------------------------------------------------------------
//Float Stack
void StackFloatInit(TSTACKFLOAT *s)
{
	s->top=STACK_EMPTY;
}

uint8 StackFloatIsEmpty(TSTACKFLOAT *s)
{
	return s->top==STACK_EMPTY;
}

uint8 StackFloatIsFull(TSTACKFLOAT *s)
{
  return s->top==STACK_SIZE_FLOAT-1;
}

uint8 StackFloatPush(TSTACKFLOAT *s,float value)
{
  if(!StackFloatIsFull(s))
  {
    s->top++;
    s->value[s->top]=value;
    return 1;
  }
  return 0;
}

uint8 StackFloatPop(TSTACKFLOAT *s,float *value)
{
	if(!StackFloatIsEmpty(s))
	{
		*value=s->value[s->top];
		s->top--;
		return 1;
	}
  UsageFault_Handler();//debug
  return 0;
}
//End Float Stack
//------------------------------------------------------------------------------------------------
#endif


