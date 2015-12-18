#include "SystemConfig.h"
#include <stdio.h>
#include <string.h>
#include "ff.h"


void MainTask(void *p_arg);

//定义信号量
OS_EVENT *SEM_dataDrive;

//定义消息邮箱
OS_EVENT *Q_dataDrive;
void *QmsgDataDrive[QmsgDataDriveNum];

// 定义堆栈
OS_STK MainTask_Stk[MainTask_StkSize];

OS_STK TSK_WriteFile_Stk[TSK_WriteFile_StkSize];
OS_STK TSK_ReadFile_Stk[TSK_ReadFile_StkSize];

//声明任务fun
void TSK_WriteFile(void *p_arg);
void TSK_ReadFile(void *p_arg);

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
		SEM_dataDrive = OSSemCreate(0);
		Q_dataDrive = OSQCreate(&QmsgDataDrive[0], QmsgDataDriveNum);
		OSQFlush(Q_dataDrive);

		OSTaskCreate(TSK_WriteFile, (void *)0, &TSK_WriteFile_Stk[TSK_WriteFile_StkSize - 1], TSK_WriteFile_Prio);
		OSTaskCreate(TSK_ReadFile, (void *)0, &TSK_ReadFile_Stk[TSK_ReadFile_StkSize - 1], TSK_ReadFile_Prio);

		OSTimeDlyHMSM(0, 0, 1, 0); /* 任务调度*/
		OSTaskSuspend(MainTask_Prio);  //挂起主任务
		//		OSTaskDel(MainTask_Prio); /* 删除主任务*/
	}
}










