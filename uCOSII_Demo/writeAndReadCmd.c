#include "SystemConfig.h"
#include <stdio.h>
#include <string.h>
#include "writeAndReadCmd.h"
#include "global.h"



//task, send read command
void TSK_NetSend(void *p_arg)
{
	char res;
	char readMsg ;

	p_arg = p_arg;
	readMsg = READ_CMD;

	while (1)
	{
		printf("net send task \n");
		res = OSQPost(Q_Msg, &readMsg);
		if (res != OS_NO_ERR)
		{
			printf("filled Que Q_Msg error, %d\n");
		}

		OSTimeDlyHMSM(0, 0, 5, 0); /* 任务调度*/
		//		OSTaskChangePrio(20, TSK_ReadFile_Prio);
	}

}


//task, send write command
void TSK_WriteCmd(void *p_arg)
{
	char res;
	char writeMsg;

	p_arg = p_arg;
	writeMsg = READ_CMD;

	while (1)
	{
		printf("write cmd task \n");

		res = OSQPost(Q_Msg, &writeMsg);
		if (res != OS_NO_ERR)
		{
			printf("filled Que Q_Msg error, %d\n");
		}

		OSTimeDlyHMSM(0, 0, 5, 0); /* 任务调度*/
		//		OSTaskChangePrio(20, TSK_ReadFile_Prio);
	}

}