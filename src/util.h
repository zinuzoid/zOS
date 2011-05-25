/*------------------------------------------------------------------------------------------------
** Program 	:	util.h
** Project	:	zOS
** CPU		:	STM32F100
** Date		:	16/3/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@gmail.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/

uint8 zstrcmp(const uint8 *s1, const uint8 *s2,uint8 len);
uint8 zstrcmpnc(const uint8 *s1, const uint8 *s2,uint8 len);
void zstrcpy(char *dst,char *src);
uint8 zstrlen(const char *str);


