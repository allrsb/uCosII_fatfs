#ifndef __WRITE_FILE_H
#define __WRITE_FILE_H

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




#endif