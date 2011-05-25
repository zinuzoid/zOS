/*------------------------------------------------------------------------------------------------
** Program 	:	usart.c
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

#include "header.h"

#include "queue.h"
#include "usart.h"

static void USARTPHY1_Init(void);
static void USARTPHY2_Init(void);
static uint8 USARTPHY1_IsTxReady(void);
static uint8 USARTPHY2_IsTxReady(void);
static void USARTPHY1_SendData(uint8 data);
static void USARTPHY2_SendData(uint8 data);

static TQUEUEU8 qSerialRx[2];
static TQUEUEU8 qSerialTx[2];

//------------------------------------------------------------------------------------------------
//Application Layer
void USARTAPI_Init(void)
{
  QueueU8Init(&qSerialRx[0]);
  QueueU8Init(&qSerialTx[0]);
  QueueU8Init(&qSerialRx[1]);
  QueueU8Init(&qSerialTx[1]);
  USARTPHY1_Init();
  USARTPHY2_Init();
}

void zprint(char *str)
{
  USART1_SendStr(str);
}

uint8 USART1_RecvChar(uint8 *data)
{
  if(QueueU8Delete(&qSerialRx[0],data))
    return 1;
  return 0;
}

uint8 USART2_RecvChar(uint8 *data)
{
  if(QueueU8Delete(&qSerialRx[1],data))
    return 1;
  return 0;
}

uint8 USART1_SendChar(uint8 data)
{
  if(QueueU8Insert(&qSerialTx[0],data))
  {
    if(USARTPHY1_IsTxReady())
    {
        uint8 tmp;
        QueueU8Delete(&qSerialTx[0],&tmp);
        USARTPHY1_SendData(tmp);
        return 1;
    }
  }
  return 0;
}

uint8 USART2_SendChar(uint8 data)
{
  if(QueueU8Insert(&qSerialTx[1],data))
  {
    if(USARTPHY2_IsTxReady())
    {
        uint8 tmp;
        QueueU8Delete(&qSerialTx[1],&tmp);
        USARTPHY2_SendData(tmp);
        return 1;
    }
  }
  return 0;
}

void USART1_SendStr(char *data)
{
  while(*data)
  {
    USART1_SendChar(*data);
    data++;
  }
}

void USART2_SendStr(char *data)
{
  while(*data)
  {
    USART2_SendChar(*data);
    data++;
  }
}
//end Application Layer
//------------------------------------------------------------------------------------------------
//Middle Layer

//end Middle Layer
//------------------------------------------------------------------------------------------------
//Physical Driver
static void USARTPHY1_Init(void)
{
  GPIO_InitTypeDef GPIOInit;
  USART_InitTypeDef USARTInit;
  NVIC_InitTypeDef NVICInit;	

  //RCC
  RCC_APB2PeriphClockCmd(
                         RCC_APB2Periph_USART1|
                         RCC_APB2Periph_AFIO|
                         RCC_APB2Periph_GPIOA
                         ,ENABLE);

  //GPIO
  GPIOInit.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIOInit.GPIO_Pin=GPIO_Pin_9;
  GPIOInit.GPIO_Speed=GPIO_Speed_10MHz;
  GPIO_Init(GPIOA,&GPIOInit);
  
  GPIOInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIOInit.GPIO_Pin=GPIO_Pin_10;
  GPIOInit.GPIO_Speed=GPIO_Speed_10MHz;
  GPIO_Init(GPIOA,&GPIOInit);

  //USART
  USARTInit.USART_BaudRate=115200;
  USARTInit.USART_WordLength=USART_WordLength_8b;
  USARTInit.USART_StopBits=USART_StopBits_1;
  USARTInit.USART_Parity=USART_Parity_No;
  USARTInit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
  USARTInit.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;

  USART_Init(USART1,&USARTInit);
  USART_Cmd(USART1,ENABLE);
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  USART_ITConfig(USART1,USART_IT_TC,ENABLE);

  //IRQ
  NVICInit.NVIC_IRQChannel=USART1_IRQn;
  NVICInit.NVIC_IRQChannelPreemptionPriority=0;
  NVICInit.NVIC_IRQChannelSubPriority=5;
  NVICInit.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVICInit);
}

static void USARTPHY2_Init(void)
{
  GPIO_InitTypeDef GPIOInit;
	USART_InitTypeDef USARTInit;
	NVIC_InitTypeDef NVICInit;	

  //RCC
  RCC_APB2PeriphClockCmd(
                         RCC_APB2Periph_AFIO|
                         RCC_APB2Periph_GPIOA
                         ,ENABLE);
  RCC_APB1PeriphClockCmd(
                         RCC_APB1Periph_USART2
                         ,ENABLE);

  //GPIO
  GPIOInit.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIOInit.GPIO_Pin=GPIO_Pin_2;
  GPIOInit.GPIO_Speed=GPIO_Speed_10MHz;
  GPIO_Init(GPIOA,&GPIOInit);
  
  GPIOInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIOInit.GPIO_Pin=GPIO_Pin_3;
  GPIOInit.GPIO_Speed=GPIO_Speed_10MHz;
  GPIO_Init(GPIOA,&GPIOInit);

  //USART
  USARTInit.USART_BaudRate=115200;
  USARTInit.USART_WordLength=USART_WordLength_8b;
  USARTInit.USART_StopBits=USART_StopBits_1;
  USARTInit.USART_Parity=USART_Parity_No;
  USARTInit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
  USARTInit.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;

  USART_Init(USART2,&USARTInit);
  USART_Cmd(USART2,ENABLE);
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
  USART_ITConfig(USART2,USART_IT_TC,ENABLE);

  //IRQ
  NVICInit.NVIC_IRQChannel=USART2_IRQn;
  NVICInit.NVIC_IRQChannelPreemptionPriority=0;
  NVICInit.NVIC_IRQChannelSubPriority=5;
  NVICInit.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVICInit);
}

static uint8 USARTPHY1_IsTxReady(void)
{
  return USART_GetFlagStatus(USART1,USART_FLAG_TXE)==SET;
}

static uint8 USARTPHY2_IsTxReady(void)
{
  return USART_GetFlagStatus(USART2,USART_FLAG_TXE)==SET;
}

static void USARTPHY1_SendData(uint8 data)
{
  USART_SendData(USART1,data);
}

static void USARTPHY2_SendData(uint8 data)
{
  USART_SendData(USART2,data);
}

void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
  {
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    QueueU8Insert(&qSerialRx[0],USART_ReceiveData(USART1));
  }
  if(USART_GetITStatus(USART1,USART_IT_TC)==SET)
  {
    uint8 tmp;
    USART_ClearITPendingBit(USART1,USART_IT_TC);
    if(QueueU8Delete(&qSerialTx[0],&tmp))
      USART_SendData(USART1,tmp);
  }
}

void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
  {
    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    QueueU8Insert(&qSerialRx[1],USART_ReceiveData(USART2));
  }
  if(USART_GetITStatus(USART2,USART_IT_TC)==SET)
  {
    uint8 tmp;
    USART_ClearITPendingBit(USART2,USART_IT_TC);
    if(QueueU8Delete(&qSerialTx[1],&tmp))
      USART_SendData(USART2,tmp);
  }
}
//end Physical Driver
//------------------------------------------------------------------------------------------------


