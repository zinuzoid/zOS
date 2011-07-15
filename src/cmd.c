/*------------------------------------------------------------------------------------------------
** Program 	:	cmd.c
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	11/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

#include "header.h"

#include "usart.h"
#include "cmd.h"
#include "roundrobintask.h"
#include "util.h"

#include <stdlib.h>

#define ASCII_ESC       	27
#define ASCII_ENTER 			13
//#define ASCII_BS          8
#define ASCII_BS          0x7f
#define ASCII_CTRL_Z			26
#define ASCII_CLR_SCREEN	12

static void CmdManage_Init(TCMD *cmd,TPRINT print,const TCMD_TABLE *table,const char *prompt);
static uint8 Cmd_Help(uint8 *cmd,uint8 len,TPRINT print);
static uint8 Cmd_TaskMonitor(uint8 *cmd,uint8 len,TPRINT print);
static uint8 TCMD_BUF_IsFull(TCMD *cmd);
static uint8 TCMD_BUF_IsEmpty(TCMD *cmd);
static void CmdManage_Exe(TCMD *cmd);
static uint8 CmdManage_Check(TCMD *cmd);
static void CmdManage_Clear(TCMD *cmd);

TTASK cmdtask;

TCMD cmd;
TCMD_BUF cmdbuff;

TCMD_TABLE cmdtable[]={
  {"Reserved",0},
  {"help",Cmd_Help},
  {"task",Cmd_TaskMonitor},
  {"",0}//for end check
};

//------------------------------------------------------------------------------------------------
//Application Layer
void CmdTask_10ms(void *param)
{
  if(!param)
    return;
  
  uint8 (*RecvCharFn)(uint8*);
  RecvCharFn=(uint8(*)(uint8*))param;
  
  uint8 ch;
  uint8 i=0;
  while(RecvCharFn(&ch))
  {
    KickDog();
    CmdManage_Recv(&cmd,ch);
    //zprint((char*)&ch);
    if(i>64)  break;
    i++;
  }
}

void Cmd_Init(void)
{
  CmdManage_Init(&cmd,zprint,cmdtable,"z> ");
  TaskAdd(&cmdtask,"CMD",10,CmdTask_10ms,(void*)USART1_RecvChar);
}

void CmdManage_Recv(TCMD *cmd,uint8 ch)
{
  if(!cmd)
    return;

  switch(ch)
  {
  case ASCII_ENTER:
    if(!TCMD_BUF_IsEmpty(cmd))
    {
      cmd->buf.cmd[cmd->buf.index]='\0';
      cmd->buf.index++;
      CmdManage_Exe(cmd);
      CmdManage_Clear(cmd);
    }
      cmd->print("\r\n\r\n");
      cmd->print((char*)cmd->prompt);
    break;
  case ASCII_BS:
    if(cmd->buf.index)
    {
      cmd->print("\b \b");
      cmd->buf.index--;
    }
    break;
  default:
    if(!TCMD_BUF_IsFull(cmd))
    {
      cmd->buf.cmd[cmd->buf.index]=ch;
      cmd->buf.index++;
      cmd->buf.cmd[cmd->buf.index]=0;//terminate
      cmd->print((char*)&ch);//echo
    }
    break;
  }
}

static void CmdManage_Exe(TCMD *cmd)
{
  if(!cmd)
    return;

  uint8 idx;
  idx=CmdManage_Check(cmd);
  if(idx)
  {
    cmd->table[idx].fncmd(cmd->buf.cmd,cmd->buf.index,cmd->print);
  }
  else
  {
    cmd->print("\r\nInvalid Command.");
  }
}

static uint8 CmdManage_Check(TCMD *cmd)
{
  uint8 idx;

  if(!cmd)
    return 0;

  for(idx=1;cmd->table[idx].cmd[0];idx++)
  {
    if(zstrcmpnc(cmd->buf.cmd,(uint8*)cmd->table[idx].cmd,zstrlen(cmd->table[idx].cmd)))
      return idx;
  }

  return 0;
}

static void CmdManage_Clear(TCMD *cmd)
{
  if(!cmd)
    return;

  cmd->buf.index=0;
}

static uint8 TCMD_BUF_IsFull(TCMD *cmd)
{
  if(cmd->buf.index < CMD_MAX-2)
    return 0;
  return 1;
}

static uint8 TCMD_BUF_IsEmpty(TCMD *cmd)
{
  if(cmd->buf.index==0) return 1;
  return 0;
}

static void CmdManage_Init(TCMD *cmd,TPRINT print,const TCMD_TABLE *table,const char *prompt)
{
  if(!cmd)
    return;
  if(!print)
    return;
  if(!table)
    return;

  cmd->print=print;
  cmd->table=table;
  cmd->buf.index=0;
  cmd->prompt=prompt;
}

//------------------------------------------------------------------------------------------------
//Command Function
static uint8 Cmd_Help(uint8 *cmd,uint8 len,TPRINT print)
{
  print("\r\n+-------------------------------------------------------------------+");
  print("\r\n+ Command List                                                      +");
  print("\r\n+-------------------------------------------------------------------+");
  
  uint8 i;
  for(i=1;cmdtable[i].cmd[0];i++)
  {
    print("\r\n ");
    print((char*)cmdtable[i].cmd);
  }
  
  print("\r\n+-------------------------------------------------------------------+");
  return 1;
}
static uint8 Cmd_TaskMonitor(uint8 *cmd,uint8 len,TPRINT print)
{
  TaskMonitor(print);
  return 1;
}

static uint8 Cmd_PWMDuty(uint8 *cmd,uint8 len,TPRINT print)
{
  //duty 45
  //012345
  int32 duty;
  if(len>(4+2))
  {
    cmd+=4+1;
    duty=atoi((char*)cmd);
    //PWM_setDuty(duty);
  }
  else
  {
    //duty=PWM_getDuty();
  }
  
  char bf[8];
  //sprintf(bf,"\r\nPWMDuty=%d",duty);
  print(bf);
  
  return 1;
}
//end Command Function
//------------------------------------------------------------------------------------------------







