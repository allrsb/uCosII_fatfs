#include "system_config.h"
#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "SD_work.h"
#include "write_read_cmd.h"
#include "global.h"
#include "debug.h"

#define DEBUG

#define FSN_PATH         "FSN"
#define TXT_PATH         "TXT"
#define FSN_UNLOAD_PATH  "FSN/UNLOAD"
#define FSN_LOADED_PATH  "FSN/LOADED"
#define TXT_UNLOAD_PATH  "TXT/UNLOAD"
#define TXT_LOADED_PATH  "TXT/LOADED"

static int SD_SystemInit(void);
static int SD_DirInit(void);
static int GetLocalDataTime(LocalTim *);
int FindVaildFile(char *dirPath, char * matchFile, char *dstName);
static int traverse_subcatalog(char *directory_path, sd_file_manage_t * p_sd_file_manage, char *pattern);

FATFS fs;            // Work area (file system object) for logical drive
char WriteDataBuf[10 * 1024 * 1024];
char wFilePath[32] = { 0 };
char wFsnUnloadDirPath[32] = { 0 };
char wTxtUnloadDirPath[32] = { 0 };
char wFsnLoadedDirPath[32] = { 0 };
char wTxtLoadedDirPath[32] = { 0 };

WriteBuffer gWritebuf;
sd_file_manage_t sd_file_manage;

void TSK_SDWork()
{
	FRESULT res;
	unsigned char err;
	FIL fsrc, fdst;       // file objects
	LocalTim dataTime;
	int bw = 0;
	char * pCmd;
	char rdName[13] = { 0 };

	SD_SystemInit();

	while (1)
	{
		pCmd = OSQPend(Q_Msg, 0, &err);
		DBG_print(DBG_INFO, "que cmd = %d", *pCmd);

		while (1)
		{
			switch (*pCmd)
			{
			case WRITE_CMD:
				GetLocalDataTime(&dataTime);
				/*write fsn file*/
				sprintf(wFsnUnloadDirPath, "FSN/UNLOAD/%04d%02d%02d", dataTime.year, dataTime.mon, dataTime.day);
				sprintf(wFsnLoadedDirPath, "FSN/LOADED/%04d%02d%02d", dataTime.year, dataTime.mon, dataTime.day);
				res = f_mkdir(wFsnUnloadDirPath);
				if (res != FR_OK && res != FR_EXIST)
				{
					DBG_print(DBG_ERROR, "mkdir %s err, %d", wFsnUnloadDirPath, res);
				}
				res = f_mkdir(wFsnLoadedDirPath);
				if (res != FR_OK && res != FR_EXIST)
				{
					DBG_print(DBG_ERROR, "mkdir %s err, %d", wFsnLoadedDirPath, res);
				}
				sprintf(wFilePath, "%s/%02d%02d%02d.fsn", wFsnUnloadDirPath, dataTime.hour, dataTime.tmin, dataTime.sec);
				res = f_open(&fdst, wFilePath, FA_CREATE_ALWAYS | FA_WRITE);
				if (res != FR_OK)
				{
					DBG_print(DBG_ERROR, "creat file err, %d", res);
				}

				res = f_write(&fdst, gWritebuf.wbuf, gWritebuf.length, &bw);
				if (res != FR_OK || bw != gWritebuf.length)
				{
					DBG_print(DBG_ERROR, "write file err, %d, %d", res, bw);
				}
				f_close(&fdst);
				DBG_print(DBG_INFO, "creat & write file %s", wFilePath);


				/*write txt file*/
				sprintf(wTxtUnloadDirPath, "TXT/UNLOAD/%04d%02d%02d", dataTime.year, dataTime.mon, dataTime.day);
				sprintf(wTxtLoadedDirPath, "TXT/LOADED/%04d%02d%02d", dataTime.year, dataTime.mon, dataTime.day);
				res = f_mkdir(wTxtUnloadDirPath);
				if (res != FR_OK && res != FR_EXIST)
				{
					DBG_print(DBG_ERROR, "mkdir %s err, %d", wTxtUnloadDirPath, res);
				}
				res = f_mkdir(wTxtLoadedDirPath);
				if (res != FR_OK && res != FR_EXIST)
				{
					DBG_print(DBG_ERROR, "mkdir %s err, %d", wTxtLoadedDirPath, res);
				}

				sprintf(wFilePath, "%s/%02d%02d%02d.txt", wTxtUnloadDirPath, dataTime.hour, dataTime.tmin, dataTime.sec);
				res = f_open(&fdst, wFilePath, FA_CREATE_ALWAYS | FA_WRITE);
				if (res != FR_OK)
				{
					DBG_print(DBG_ERROR, "creat file err, %d", res);
				}

				res = f_write(&fdst, gWritebuf.wbuf, gWritebuf.length, &bw);
				if (res != FR_OK || bw != gWritebuf.length)
				{
					DBG_print(DBG_ERROR, "write file err, %d, %d", res, bw);
				}
				f_close(&fdst);

				DBG_print(DBG_INFO, "creat & write file %s", wFilePath);
				break;
			case READ_CMD:
				if (0 == wFsnUnloadDirPath[0])
				{
					DBG_print(DBG_ERROR, "unload path is null");
					break;
				}

				FindVaildFile(wFsnUnloadDirPath, L"*.FSN", rdName);
//				sprintf();
//				res = f_open(&fd, "FSN\\UNLOAD\\154623.FSN", FA_READ | FA_OPEN_EXISTING);
				if (res != FR_OK)
				{
					DBG_print(DBG_ERROR, "open file err, %d", res);
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
int SD_SystemInit(void)
{
	FRESULT res;
	FIL fsrc;       // file objects
	unsigned int br;

	memset(&gWritebuf, 0, sizeof(gWritebuf));
	gWritebuf.wbuf = WriteDataBuf;
	f_mount(&fs, "", 0);  // Register a work area to logical drive 0
	
	/*缓存将要写的数据*/
	res = f_open(&fsrc, "wdata.fsn", FA_READ | FA_OPEN_EXISTING);
	if (res != FR_OK)
	{
		DBG_print(DBG_ERROR, "open file err, %d", res);
		goto err1;
	}
	res = f_read(&fsrc, gWritebuf.wbuf, fsrc.fsize, &gWritebuf.length);
	if (res != FR_OK)
	{
		DBG_print(DBG_ERROR, "read file err, %d", res);
		goto err1;
	}
	gWritebuf.validSign = 1;
	res = f_close(&fsrc);
	if (res != FR_OK)
	{
		DBG_print(DBG_ERROR, "close file err, %d", res);
		goto err1;
	}

	/*创建目录结构*/
	res = f_mkdir(FSN_PATH);
	if (res != FR_OK && res != FR_EXIST)
	{
		DBG_print(DBG_ERROR, "mkdir %s err, %d", FSN_UNLOAD_PATH, res);
		goto err2;
	}
	res = f_mkdir(TXT_PATH);
	if (res != FR_OK && res != FR_EXIST)
	{
		DBG_print(DBG_ERROR, "mkdir %s err, %d", FSN_UNLOAD_PATH, res);
		goto err2;
	}
	res = f_mkdir(FSN_UNLOAD_PATH);
	if (res != FR_OK && res != FR_EXIST)
	{
		DBG_print(DBG_ERROR, "mkdir %s err, %d", FSN_UNLOAD_PATH, res);
		goto err2;
	}
	res = f_mkdir(FSN_LOADED_PATH);
	if (res != FR_OK && res != FR_EXIST)
	{
		DBG_print(DBG_ERROR, "mkdir %s err, %d", FSN_LOADED_PATH, res);
		goto err2;
	}
	res = f_mkdir(TXT_UNLOAD_PATH);
	if (res != FR_OK && res != FR_EXIST)
	{
		DBG_print(DBG_ERROR, "mkdir %s err, %d", TXT_UNLOAD_PATH, res);
		goto err2;
	}
	res = f_mkdir(TXT_LOADED_PATH);
	if (res != FR_OK && res != FR_EXIST)
	{
		DBG_print(DBG_ERROR, "mkdir %s err, %d", TXT_LOADED_PATH, res);
		goto err2;
	}

	traverse_subcatalog("\\FSN\\UNLOAD", &sd_file_manage, "*");
//	traverse_subcatalog("\\FSN\\UNLOAD\\20160309", &sd_file_manage, L"*.*");

	return 0;

err1:
	DBG_print(DBG_ERROR, "prepare write file buffer err");
	return 1;
err2:
	DBG_print(DBG_ERROR, "make dirs err");
	return 2;
	
}


/*********************************************
*@brief  在文件夹中遍历子文件夹，返回最后文件夹名字指针
*@param [in ]  directory_path
*@param [out]  directory_name
*@param [in ]  pattern 匹配模式，比如“*.fsn”
*@return
**********************************************
*/
static char traverse_subcatalog(char *directory_path, char *directory_name, char *pattern)
{
	int res;
	DIR directory_object;
	FILINFO file_object;

	char tst[12];
#if _USE_LFN
	char lfn[_MAX_LFN + 1];
	file_object.lfname = lfn;
	file_object.lfsize = _MAX_LFN + 1;
#endif

	res = f_findfirst(&directory_object, &file_object, directory_path, pattern);
	while (res == FR_OK && file_object.fname[0])
	{
#if _USE_LFN
		DBG_print(DBG_DEBUG, "%-12s  %s", file_object.fname, file_object.lfname);
#else
		
		DBG_print(DBG_DEBUG, "%s", (file_object.fname));
		
#endif
		res = f_findnext(&directory_object, &file_object);
	}

	if (0 != file_object.fname[0])   
		directory_name = file_object.fname;

	return 0;
}

/*********************************************
*@brief  在文件夹中遍历子文件夹，返回最后文件夹名字指针
*@param [in ]  directory_path
*@param [out]  directory_name
*@param [in ]  pattern 匹配模式，比如“*.fsn”
*@return
**********************************************
*/
static char traverse_subfile(char *directory_path, char *directory_name, char *pattern)
{

}



//get loacal time
int GetLocalDataTime(LocalTim * dt)
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
	if (res == FR_OK && fno.fname[0])
	{
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