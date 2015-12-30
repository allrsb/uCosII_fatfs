#include "SystemConfig.h"
#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "writeFile.h"
#include "writeAndReadCmd.h"
#include "global.h"

#define DEBUG

#define FSN_PATH         "FSN"
#define TXT_PATH         "TXT"
#define FSN_UNLOAD_PATH  "FSN/UNLOAD"
#define FSN_LOADED_PATH  "FSN/LOADED"
#define TXT_UNLOAD_PATH  "TXT/UNLOAD"
#define TXT_LOADED_PATH  "TXT/LOADED"

static void WriteInit(void);
static int GetLocalTim(LocalTim *);
int FindVaildFile(char *dirPath, char * matchFile, char *dstName);

FATFS fs;            // Work area (file system object) for logical drive
char WriteDataBuf[10 * 1024 * 1024];
char wFilePath[32] = { 0 };
char wFsnUnloadDirPath[32] = { 0 };
char wTxtUnloadDirPath[32] = { 0 };
char wFsnLoadedDirPath[32] = { 0 };
char wTxtLoadedDirPath[32] = { 0 };

WriteBuffer gWritebuf;


void TSK_WriteFile()
{
	FRESULT res;
	unsigned char err;
	FIL fsrc, fdst;       // file objects
	LocalTim dataTime;
	int bw = 0;
	char * pCmd;
	char rdName[13] = { 0 };

	WriteInit();

	while (1)
	{
		pCmd = OSQPend(Q_Msg, 0, &err);
		printf("que cmd = %d\n", *pCmd);

		while (1)
		{
			switch (*pCmd)
			{
			case WRITE_CMD:
				GetLocalTim(&dataTime);
				/*write fsn file*/
				sprintf(wFsnUnloadDirPath, "FSN/UNLOAD/%04d%02d%02d", dataTime.year, dataTime.mon, dataTime.day);
				sprintf(wFsnLoadedDirPath, "FSN/LOADED/%04d%02d%02d", dataTime.year, dataTime.mon, dataTime.day);
				res = f_mkdir(wFsnUnloadDirPath);
				if (res != FR_OK && res != FR_EXIST)
				{
					printf("mkdir %s err, %d\n", wFsnUnloadDirPath, res);
				}
				res = f_mkdir(wFsnLoadedDirPath);
				if (res != FR_OK && res != FR_EXIST)
				{
					printf("mkdir %s err, %d\n", wFsnLoadedDirPath, res);
				}
				sprintf(wFilePath, "%s/%02d%02d%02d.fsn", wFsnUnloadDirPath, dataTime.hour, dataTime.tmin, dataTime.sec);
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
				sprintf(wTxtUnloadDirPath, "TXT/UNLOAD/%04d%02d%02d", dataTime.year, dataTime.mon, dataTime.day);
				sprintf(wTxtLoadedDirPath, "TXT/LOADED/%04d%02d%02d", dataTime.year, dataTime.mon, dataTime.day);
				res = f_mkdir(wTxtUnloadDirPath);
				if (res != FR_OK && res != FR_EXIST)
				{
					printf("mkdir %s err, %d\n", wTxtUnloadDirPath, res);
				}
				res = f_mkdir(wTxtLoadedDirPath);
				if (res != FR_OK && res != FR_EXIST)
				{
					printf("mkdir %s err, %d\n", wTxtLoadedDirPath, res);
				}

				sprintf(wFilePath, "%s/%02d%02d%02d.txt", wTxtUnloadDirPath, dataTime.hour, dataTime.tmin, dataTime.sec);
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
				break;
			case READ_CMD:
				if (0 == wFsnUnloadDirPath[0])
				{
					printf("unload path is null\n");
					break;
				}

				FindVaildFile(wFsnUnloadDirPath, L"*.FSN", rdName);
//				sprintf();
//				res = f_open(&fd, "FSN\\UNLOAD\\154623.FSN", FA_READ | FA_OPEN_EXISTING);
				if (res != FR_OK)
				{
					printf("open file err, %d\n", res);
				}
				//				FindVaildFile(wFsnUnloadDirPath, "*.fsn");
				break;
			default:
				break;
			}

			break;
		}

		//		OSTimeDlyHMSM(0, 0, 0, 0); /* 任务调度*/

	}
}

//fill buffer, make dirs
void WriteInit(void)
{
	FRESULT res;
	FIL fsrc;       // file objects
	unsigned int br;

	memset(&gWritebuf, 0, sizeof(gWritebuf));
	gWritebuf.wbuf = WriteDataBuf;


	f_mount(&fs, "", 0);  // Register a work area to logical drive 0



	res = f_open(&fsrc, _T("wdata.fsn"), FA_READ | FA_OPEN_EXISTING);
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

	res = f_close(&fsrc);




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

//get loacal time
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

/*get queue such as read or write cmd*/
int GetQueueCmd(void)
{

	return 0;
}

/*find a vaild .fsn or .txt file*/
int FindVaildFile(char *dirPath, char * matchFile, char *dstName)
{
	FRESULT res;
	DIR dj;
	FILINFO fno;
#if _USE_LFN
	char lfn[_MAX_LFN + 1];
	fno.lfname = lfn;
	fno.lfsize = _MAX_LFN + 1;
#endif
	res = f_findfirst(&dj, &fno, dirPath, matchFile);
	if (res == FR_OK && fno.fname[0]){
		f_closedir(&dj);
		strcpy(dstName, fno.fname);
		return 0;
	}
	f_closedir(&dj);
	return 1;


	//	while (res == FR_OK && fno.fname[0]) {         /* Repeat while an item is found */
	//#if _USE_LFN
	//		printf("%-12s  %s\n", fno.fname, fno.lfname);/* Display the item name */
	//#else
	//		printf("%s\n", fno.fname);
	//#endif
	//		res = f_findnext(&dj, &fno);               /* Search for next item */
	//	}
	//	f_closedir(&dj);

}