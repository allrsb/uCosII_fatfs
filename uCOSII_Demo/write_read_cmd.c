#include "system_config.h"
#include <stdio.h>
#include <string.h>
#include "write_read_cmd.h"
#include "global.h"
#include "debug.h"



//task, send read command
void TSK_NetSend(void *p_arg)
{
	char res;
	char readMsg ;

	p_arg = p_arg;
	readMsg = READ_CMD;

	while (1)
	{
		DBG_print(DBG_INFO, "NetSend task send read cmd");
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
	writeMsg = WRITE_CMD;

	while (1)
	{
		DBG_print(DBG_INFO, "WriteCmd task send write cmd");

		res = OSQPost(Q_Msg, &writeMsg);
		if (res != OS_NO_ERR)
		{
			printf("filled Que Q_Msg error, %d\n");
		}

		OSTimeDlyHMSM(0, 0, 5, 0); /* 任务调度*/
//      OSTaskChangePrio(20, TSK_ReadFile_Prio);
	}

}