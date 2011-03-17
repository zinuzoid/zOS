/*------------------------------------------------------------------------------------------------
** Program 	:	cmd.h
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

#ifndef _CMD_H
#define _CMD_H

#define CMD_MAX 200

typedef void (*TPRINT)(char*);

typedef struct _TCMD_TABLE
{
  const char *cmd;
  uint8 (*fncmd)(uint8*,uint8,TPRINT);//ch,len,strout
}TCMD_TABLE;

typedef struct _TCMD_BUF
{
  uint8 cmd[CMD_MAX];
  uint8 index;
}TCMD_BUF;

typedef struct _TCMD
{
  TCMD_BUF buf;
  TPRINT print;
  const TCMD_TABLE *table;
  const char *prompt;
}TCMD;

void Cmd_Init(void);
void CmdManage_Recv(TCMD *cmd,uint8 ch);

#endif


