/*------------------------------------------------------------------------------------------------
** Program 	:	usart.h
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

void USARTAPI_Init(void);
uint8 USART1_RecvChar(uint8 *data);
uint8 USART2_RecvChar(uint8 *data);
uint8 USART1_SendChar(uint8 data);
uint8 USART2_SendChar(uint8 data);
void USART1_SendStr(char *data);
void USART2_SendStr(char *data);

void zprint(char *str);



