#include "SystemConfig.h"
#include <stdio.h>
#include <string.h>
#include "ff.h"

FATFS fs;            // Work area (file system object) for logical drive
char gWriteDataBuf[1024 * 10];


void TSK_WriteFile()
{
	FRESULT res;
	FIL fsrc, fdst;       // file objects
	
	memset(gWriteDataBuf, 0, sizeof(gWriteDataBuf));

	f_mount(&fs, "", 0);  // Register a work area to logical drive 0

	while (1)
	{
		res = f_open(&fdst, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);


		f_close(&fdst);
	}


}