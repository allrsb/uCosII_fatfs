#include "system_config.h"
#include <stdio.h>
#include <string.h>
#include "write_read_cmd.h"
#include "global.h"
#include "debug.h"
#include "SD_work.h"

sd_message_t sd_message_net[NET_MSG_MAX_NUM] = {0};
sd_message_t sd_message_buffer[BUF_MSG_MAX_NUM] = {0};

void TSK_NetSend(void *p_arg)
{
	char res;
	int i;

	p_arg = p_arg;

	while (1)
	{
		DBG_print(DBG_INFO, "NetSend task send read cmd");
		for (i = 0; i < NET_MSG_MAX_NUM; i ++)
		{
			if (sd_message_net[i].valid_sign == 0)
			{
				sd_message_net[i].type = READ_CMD;
				sd_message_net[i].priority = MSG_READ_PRIORITY;
				sd_message_net[i].valid_sign = 1;

				res = OSQPost(Q_Msg, &sd_message_net[i]);
				if (res != OS_NO_ERR)
				{
					printf("filled Que Q_Msg error, %d\n");
				}
				break;
			}
		}

		OSTimeDlyHMSM(0, 0, 5, 0); // 任务调度
		
	}

}

void TSK_WriteCmd(void *p_arg)
{
	char res;
	int i;
	p_arg = p_arg;

	while (1)
	{
		DBG_print(DBG_INFO, "WriteCmd task send write cmd");

		for (i = 0; i < BUF_MSG_MAX_NUM; i++)
		{
			if (sd_message_buffer[i].valid_sign == 0)
			{
				sd_message_buffer[i].type = WRITE_CMD;
				sd_message_buffer[i].priority = MSG_READ_PRIORITY;
				sd_message_buffer[i].valid_sign = 1;

				res = OSQPost(Q_Msg, &sd_message_buffer[i]);
				if (res != OS_NO_ERR)
				{
					printf("filled Que Q_Msg error, %d\n");
				}
				break;
			}
		}

		OSTimeDlyHMSM(0, 0, 5, 0); // 任务调度

	}
}

/*
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

		OSTimeDlyHMSM(0, 0, 5, 0); // 任务调度
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

		OSTimeDlyHMSM(0, 0, 5, 0); // 任务调度
//      OSTaskChangePrio(20, TSK_ReadFile_Prio);
	}

}
*/