#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#define _CRT_SECURE_NO_WARNINGS	

#include <stdio.h>
#include <assert.h>
#include <includes.h>		



/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define MainTask_Prio 5


#define TSK_SDWork_Prio    10
#define TSK_ReadFile_Prio     12
#define TSK_NetSend_Prio      8
#define TSK_WriteCmd_Prio     9


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

#define MainTask_StkSize 1024

//for test
#define TSK_SDWork_StkSize     1024
#define TSK_ReadFile_StkSize      1024
#define TSK_NetSend_StkSize       1024
#define TSK_WriteCmd_StkSize      1024


#define OS_TASK_TMR_PRIO 65533
#define OS_IDLE_PRIO OS_TASK_IDLE_PRIO
#define OS_STAT_PRIO OS_TASK_STAT_PRIO

/*
*********************************************************************************************************
*                                            Qmsg
*********************************************************************************************************
*/
#define QmsgStrNum   32
//#define QwriteCmdFileStrNum  32



//extern OS_EVENT *SEM_NetReadFile;
//extern OS_EVENT *SEM_WriteCmdFile;



#endif