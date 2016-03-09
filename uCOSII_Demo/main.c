#include "system_config.h"
#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "debug.h"


void MainTask(void *p_arg);

//�����ź���
OS_EVENT *SEM_NetReadFile;
OS_EVENT *SEM_WriteCmdFile;

//������Ϣ����
OS_EVENT *Q_Msg;
void *QmsgStr[QmsgStrNum];
//OS_EVENT *Q_WriteCmdFile;
//void *QwriteCmdFileStr[QwriteCmdFileStrNum];

// �����ջ
OS_STK MainTask_Stk[MainTask_StkSize];
OS_STK TSK_SDWork_Stk[TSK_SDWork_StkSize];
OS_STK TSK_ReadFile_Stk[TSK_ReadFile_StkSize];
OS_STK TSK_NetSend_Stk[TSK_NetSend_StkSize];
OS_STK TSK_WriteCmd_Stk[TSK_WriteCmd_StkSize];

//��������fun
void TSK_SDWork(void *p_arg);
void TSK_ReadFile(void *p_arg);
void TSK_NetSend(void *p_arg);
void TSK_WriteCmd(void *p_arg);


int main(void)
{
	OSInit(); /* ϵͳ��ʼ��*/
	/* ����������*/
	OSTaskCreate(MainTask, (void *)0, &MainTask_Stk[MainTask_StkSize - 1], MainTask_Prio);
	OSStart(); /* ��ʼ�������*/
	return 0;
}

/* ������*/
void MainTask(void *p_arg)
{
	p_arg = p_arg;

	OSStatInit(); /* ͳ�������ʼ��*/
	while (1)
	{
		/* ������������*/
		SEM_NetReadFile = OSSemCreate(0);
		SEM_WriteCmdFile = OSSemCreate(0);

		//������Ϣ����
		Q_Msg = OSQCreate(&QmsgStr[0], QmsgStrNum);
		OSQFlush(Q_Msg);
//		Q_NetReadFile = OSQCreate(&QwriteCmdFileStr[0], QwriteCmdFileStrNum);
//		OSQFlush(Q_NetReadFile);

		OSTaskCreate(TSK_SDWork, (void *)0, &TSK_SDWork_Stk[TSK_SDWork_StkSize - 1], TSK_SDWork_Prio);
//		OSTaskCreate(TSK_ReadFile, (void *)0, &TSK_ReadFile_Stk[TSK_ReadFile_StkSize - 1], TSK_ReadFile_Prio);
		OSTaskCreate(TSK_NetSend, (void *)0, &TSK_NetSend_Stk[TSK_NetSend_StkSize - 1], TSK_NetSend_Prio);
		OSTaskCreate(TSK_WriteCmd, (void *)0, &TSK_WriteCmd_Stk[TSK_WriteCmd_StkSize - 1], TSK_WriteCmd_Prio);
		
//		OSTimeDlyHMSM(0, 0, 1, 0); /* �������*/
		OSTaskSuspend(MainTask_Prio);  //����������
//      OSTaskDel(MainTask_Prio); /* ɾ��������*/
	}
}










