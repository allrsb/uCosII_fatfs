#include "SystemConfig.h"
#include <stdio.h>
#include <string.h>
#include "ff.h"

long CountAccuratetimeStart(LARGE_INTEGER *pLr, double * pfreq);
long CountAccuratetimeEnd(LARGE_INTEGER *pLr);

void TSK_ReadFile(void *p_arg)
{
	FRESULT res;
	FIL fd;       // file objects
	LARGE_INTEGER  largeInterger; // count time
	double freq;
	long  c1, c2;

	p_arg = p_arg;
	while (1)
	{		
		printf("read task \n");

		c1 = CountAccuratetimeStart(&largeInterger, &freq);
		res = f_open(&fd, "FSN\\UNLOAD\\154623.FSN", FA_READ | FA_OPEN_EXISTING);
		if (res != FR_OK)
		{
			printf("open file err, %d\n", res);
		}
		c2 = CountAccuratetimeEnd(&largeInterger);
		printf("read 154623.FSN %f ms\n", (c2 - c1) * 1000 / freq);
		f_close(&fd);

		c1 = CountAccuratetimeStart(&largeInterger, &freq);
		res = f_open(&fd, "FSN\\UNLOAD\\154623.FSN", FA_READ | FA_OPEN_EXISTING);
		if (res != FR_OK)
		{
			printf("open file err, %d\n", res);
		}
		c2 = CountAccuratetimeEnd(&largeInterger);
		printf("read 154623.FSN %f ms\n", (c2 - c1) * 1000 / freq);
		f_close(&fd);

		c1 = CountAccuratetimeStart(&largeInterger, &freq);
		res = f_open(&fd, "FSN\\UNLOAD\\143631.FSN", FA_READ | FA_OPEN_EXISTING);
		if (res != FR_OK)
		{
			printf("open file err, %d\n", res);
		}
		c2 = CountAccuratetimeEnd(&largeInterger);
		printf("read 143631.FSN %f ms\n", (c2 - c1) * 1000 / freq);
		f_close(&fd);

		OSTimeDlyHMSM(0, 0, 1, 0); /* 任务调度*/
//		OSTaskChangePrio(20, TSK_ReadFile_Prio);
	}

}

long CountAccuratetimeStart(LARGE_INTEGER *pLr, double * pfreq)
{
	OSSchedLock();
	QueryPerformanceFrequency(pLr);    //精确计算时间
	*pfreq = pLr->QuadPart;
	QueryPerformanceCounter(pLr);

	return pLr->QuadPart;
}

long CountAccuratetimeEnd(LARGE_INTEGER *pLr)
{
	QueryPerformanceCounter(pLr);
	OSSchedUnlock();

	return pLr->QuadPart;
}