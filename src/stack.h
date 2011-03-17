/*------------------------------------------------------------------------------------------------
** Program 	:	stack.h
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	17/3/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.>
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

#define _STACKFLOAT//use float stack

#ifndef _STACK_H
#define _STACK_H

#define	STACK_EMPTY	0

#define	STACK_SIZE_U8	50//max u16

typedef struct _TSTACKU8
{
	uint8 value[STACK_SIZE_U8];
	uint16 top;
}TSTACKU8;

void StackU8Init(TSTACKU8 *s);
uint8 StackU8IsEmpty(TSTACKU8 *s);
uint8 StackU8IsFull(TSTACKU8 *s);
uint8 StackU8Push(TSTACKU8 *s,uint8 value);
uint8 StackU8Pop(TSTACKU8 *s,uint8 *value);

#ifdef _STACKFLOAT

#define	STACK_SIZE_FLOAT	50//max u16

typedef struct _TSTACKFLOAT
{
	float value[STACK_SIZE_FLOAT];
	uint16 top;
}TSTACKFLOAT;

void StackFloatInit(TSTACKFLOAT *s);
uint8 StackFloatIsEmpty(TSTACKFLOAT *s);
uint8 StackFloatIsFull(TSTACKFLOAT *s);
uint8 StackFloatPush(TSTACKFLOAT *s,float value);
uint8 StackFloatPop(TSTACKFLOAT *s,float *value);

#endif

#endif


