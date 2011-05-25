/*------------------------------------------------------------------------------------------------
** Program 	:	util.c
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	16/3/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

#include "header.h"

uint8 zstrcmp(const uint8 *s1, const uint8 *s2,uint8 len)
{
  while(
        (len)&&
        (*s1!='\0')&&
        (*s2!='\0')
       )
  {
    if(*s1!=*s2)
    {
      return 0;
    }
    s1++;
    s2++;
    len--;
  }
  if((*s1=='\0')&&(*s2=='\0')||(len==0))
    return 1;
  return 0;
}

uint8 zstrcmpnc(const uint8 *s1, const uint8 *s2,uint8 len)//non case sensitive
{
  while(
        (len)&&
        (*s1!='\0')&&
        (*s2!='\0')
       )
  {
    if(!(
       (*s1==*s2)||
       (*s1==(*s2+('a'-'A')))||//non case sensitive
       (*s1==(*s2-('a'-'A')))
      ))
    {
      return 0;
    }
    s1++;
    s2++;
    len--;
  }
  if((*s1=='\0')&&(*s2=='\0')||(len==0))
    return 1;
  return 0;
}

void zstrcpy(char *dst,char *src)
{
    while(*src)
    {
      *dst=*src;
      src++;
    }
}

uint8 zstrlen(const char *str)
{
  uint8 len=0;
  while(*str)
  {
    len++;
    str++;
  }
  return len;
}





