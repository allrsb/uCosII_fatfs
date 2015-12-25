#include "SystemConfig.h"
#include <stdio.h>
#include <string.h>
#include "ff.h"


void MainTask(void *p_arg);

//�����ź���
OS_EVENT *SEM_dataDrive;

//������Ϣ����
OS_EVENT *Q_dataDrive;
void *QmsgDataDrive[QmsgDataDriveNum];

// �����ջ
OS_STK MainTask_Stk[MainTask_StkSize];

OS_STK TSK_WriteFile_Stk[TSK_WriteFile_StkSize];
OS_STK TSK_ReadFile_Stk[TSK_ReadFile_StkSize];

//��������fun
void TSK_WriteFile(void *p_arg);
void TSK_ReadFile(void *p_arg);

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
		SEM_dataDrive = OSSemCreate(0);
		Q_dataDrive = OSQCreate(&QmsgDataDrive[0], QmsgDataDriveNum);
		OSQFlush(Q_dataDrive);

		OSTaskCreate(TSK_WriteFile, (void *)0, &TSK_WriteFile_Stk[TSK_WriteFile_StkSize - 1], TSK_WriteFile_Prio);
		OSTaskCreate(TSK_ReadFile, (void *)0, &TSK_ReadFile_Stk[TSK_ReadFile_StkSize - 1], TSK_ReadFile_Prio);

//		OSTimeDlyHMSM(0, 0, 1, 0); /* �������*/
		OSTaskSuspend(MainTask_Prio);  //����������
//      OSTaskDel(MainTask_Prio); /* ɾ��������*/
	}
}










