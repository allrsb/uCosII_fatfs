#include "SystemConfig.h"
#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "writeFile.h"


#define FSN_PATH         "FSN"
#define TXT_PATH         "TXT"
#define FSN_UNLOAD_PATH  "FSN/UNLOAD"
#define FSN_LOADED_PATH  "FSN/LOADED"
#define TXT_UNLOAD_PATH  "TXT/UNLOAD"
#define TXT_LOADED_PATH  "TXT/LOADED"

static void WriteInit(void);
static int GetLocalTim(LocalTim *);

FATFS fs;            // Work area (file system object) for logical drive
char WriteDataBuf[10 * 1024 * 1024];
char wFilePath[32] = {0};

WriteBuffer gWritebuf;

void TSK_WriteFile()
{
	FRESULT res;
	FIL fsrc, fdst;       // file objects
	LocalTim dataTime;
	int bw = 0;
		
	WriteInit();


	while (1)
	{
		GetLocalTim(&dataTime);
		/*write fsn file*/
		sprintf(wFilePath, "FSN/UNLOAD/%02d%02d%02d.fsn", dataTime.hour, dataTime.tmin, dataTime.sec);
		res = f_open(&fdst, wFilePath, FA_CREATE_ALWAYS | FA_WRITE);
		if (res != FR_OK)
		{
			printf("creat file err, %d\n", res);
		}

		res = f_write(&fdst, gWritebuf.wbuf, gWritebuf.length, &bw);
		if (res != FR_OK || bw != gWritebuf.length)
		{
			printf("write file err, %d, %d", res, bw);
		}
		f_close(&fdst);

		/*write txt file*/
		sprintf(wFilePath, "TXT/UNLOAD/%02d%02d%02d.txt", dataTime.hour, dataTime.tmin, dataTime.sec);
		res = f_open(&fdst, wFilePath, FA_CREATE_ALWAYS | FA_WRITE);
		if (res != FR_OK)
		{
			printf("creat file err, %d\n", res);
		}

		res = f_write(&fdst, gWritebuf.wbuf, gWritebuf.length, &bw);
		if (res != FR_OK || bw != gWritebuf.length)
		{
			printf("write file err, %d, %d", res, bw);
		}
		f_close(&fdst);


		printf("creat & write file %s, \n", wFilePath);
		OSTimeDlyHMSM(0, 0, 100, 0); /* 任务调度*/
		


	}


}


void WriteInit(void)
{
	FRESULT res;
	FIL fsrc;       // file objects
	unsigned int br;

	memset(&gWritebuf, 0, sizeof(gWritebuf));
	gWritebuf.wbuf = WriteDataBuf;


	f_mount(&fs, "", 0);  // Register a work area to logical drive 0



	res = f_open(&fsrc, "wdata.fsn", FA_READ | FA_OPEN_EXISTING);
	if (res != FR_OK)
	{
		printf("open file err, %d\n", res);

	}

	res = f_read(&fsrc, gWritebuf.wbuf, fsrc.fsize, &gWritebuf.length);
	if (res != FR_OK)
	{
		printf("read file err, %d\n", res);
	}
	gWritebuf.validSign = 1;


	res = f_mkdir(FSN_PATH);
	if (res != FR_OK && res != FR_EXIST)
	{
		printf("mkdir %s err, %d\n", FSN_UNLOAD_PATH, res);
	}

	res = f_mkdir(TXT_PATH);
	if (res != FR_OK && res != FR_EXIST)
	{
		printf("mkdir %s err, %d\n", FSN_UNLOAD_PATH, res);
	}

	res = f_mkdir(FSN_UNLOAD_PATH);
	if (res != FR_OK && res != FR_EXIST)
	{
		printf("mkdir %s err, %d\n", FSN_UNLOAD_PATH, res);

	}
	res = f_mkdir(FSN_LOADED_PATH);
	if (res != FR_OK && res != FR_EXIST)
	{
		printf("mkdir %s err, %d\n", FSN_LOADED_PATH, res);
	}
	res = f_mkdir(TXT_UNLOAD_PATH);
	if (res != FR_OK && res != FR_EXIST)
	{
		printf("mkdir %s err, %d\n", TXT_UNLOAD_PATH, res);
	}
	res = f_mkdir(TXT_LOADED_PATH);
	if (res != FR_OK && res != FR_EXIST)
	{
		printf("mkdir %s err, %d\n", TXT_LOADED_PATH, res);
	}
}

int GetLocalTim(LocalTim * dt)
{
	time_t timer;
	struct tm *tblock;

	time(&timer);
	tblock = localtime(&timer);
	dt->sec = tblock->tm_sec;
	dt->tmin = tblock->tm_min;
	dt->hour = tblock->tm_hour;
	dt->day = tblock->tm_mday;
	dt->mon = tblock->tm_mon + 1;
	dt->year = tblock->tm_year + 1900;

	return 0;
}