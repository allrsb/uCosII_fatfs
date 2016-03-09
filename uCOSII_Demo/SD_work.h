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

//sd�����ļ�������Ϣ
typedef struct tag_sd_file_manage
{
	unsigned int unload_file_size;  //δ�ϴ��ļ���С
}sd_file_manage_t;


#endif