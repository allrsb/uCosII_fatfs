#include "SystemConfig.h"
#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "debug.h"


void MainTask(void *p_arg);

//定义信号量
OS_EVENT *SEM_NetReadFile;
OS_EVENT *SEM_WriteCmdFile;

//定义消息邮箱
OS_EVENT *Q_Msg;
void *QmsgStr[QmsgStrNum];
//OS_EVENT *Q_WriteCmdFile;
//void *QwriteCmdFileStr[QwriteCmdFileStrNum];

// 定义堆栈
OS_STK MainTask_Stk[MainTask_StkSize];
OS_STK TSK_SDWork_Stk[TSK_SDWork_StkSize];
OS_STK TSK_ReadFile_Stk[TSK_ReadFile_StkSize];
OS_STK TSK_NetSend_Stk[TSK_NetSend_StkSize];
OS_STK TSK_WriteCmd_Stk[TSK_WriteCmd_StkSize];

//声明任务fun
void TSK_SDWork(void *p_arg);
void TSK_ReadFile(void *p_arg);
void TSK_NetSend(void *p_arg);
void TSK_WriteCmd(void *p_arg);


int main(void)
{
	OSInit(); /* 系统初始化*/
	/* 创建主任务*/
	OSTaskCreate(MainTask, (void *)0, &MainTask_Stk[MainTask_StkSize - 1], MainTask_Prio);
	OSStart(); /* 开始任务调度*/
	return 0;
}

/* 主任务*/
void MainTask(void *p_arg)
{
	p_arg = p_arg;

	OSStatInit(); /* 统计任务初始化*/
	while (1)
	{
		/* 创建其他任务*/
		SEM_NetReadFile = OSSemCreate(0);
		SEM_WriteCmdFile = OSSemCreate(0);

		//创建消息邮箱
		Q_Msg = OSQCreate(&QmsgStr[0], QmsgStrNum);
		OSQFlush(Q_Msg);
//		Q_NetReadFile = OSQCreate(&QwriteCmdFileStr[0], QwriteCmdFileStrNum);
//		OSQFlush(Q_NetReadFile);

		OSTaskCreate(TSK_SDWork, (void *)0, &TSK_SDWork_Stk[TSK_SDWork_StkSize - 1], TSK_SDWork_Prio);
//		OSTaskCreate(TSK_ReadFile, (void *)0, &TSK_ReadFile_Stk[TSK_ReadFile_StkSize - 1], TSK_ReadFile_Prio);
		OSTaskCreate(TSK_NetSend, (void *)0, &TSK_NetSend_Stk[TSK_NetSend_StkSize - 1], TSK_NetSend_Prio);
		OSTaskCreate(TSK_WriteCmd, (void *)0, &TSK_WriteCmd_Stk[TSK_WriteCmd_StkSize - 1], TSK_WriteCmd_Prio);
		
//		OSTimeDlyHMSM(0, 0, 1, 0); /* 任务调度*/
		OSTaskSuspend(MainTask_Prio);  //挂起主任务
//      OSTaskDel(MainTask_Prio); /* 删除主任务*/
	}
}










