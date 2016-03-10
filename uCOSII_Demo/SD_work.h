#ifndef __SD_WORK_H
#define __SD_WORK_H

typedef struct tag_WriteBuffer
{
	char * wbuf;
	unsigned int length;
	char validSign;

}WriteBuffer;

typedef struct tag_LocalTim
{
	int sec;
	int tmin;
	int hour;
	int day;
	int mon;
	int year;
	int week;
}LocalTim;

//sd卡里文件管理信息
typedef struct tag_sd_file_manage
{
	unsigned int unload_file_num;   //未上传文件个数
	unsigned int unload_file_size;  //未上传文件大小	
}sd_file_manage_t;


#endif