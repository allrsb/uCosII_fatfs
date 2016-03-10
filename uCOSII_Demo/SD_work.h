#ifndef __SD_WORK_H
#define __SD_WORK_H

#define MESSAGE_NUM 24    //消息最大数目

//注：写操作未做保护，只能被格式化打断
#define		MSG_FORMAT_PRIORITY	  6
#define		MSG_DELFILE_PRIORITY  5
#define		MSG_WRITE_PRIORITY	  5
#define		MSG_UPGRADE_PRIORITY  5
#define		MSG_READ_PRIORITY	  4
#define		MSG_QUERY_PRIORITY	  3
#define		MSG_LOCK_PRIORITY	  3


//指派SD操作状态
#define		SD_DISK_WORK_NEW		1	//操作开始
//具体文件操作
#define		SD_DISK_FILE_CREATE		2   
#define		SD_DISK_FILE_WRITE		3
#define		SD_DISK_FILE_OPEN		4
#define		SD_DISK_FILE_READ		5
#define		SD_DISK_FILE_CLOSE		6
//网络发过来的操作
#define		SD_NET_MANAGE_LOCK		7	//加锁
#define		SD_NET_MANAGE_UNLOCK	8	//解锁
#define		SD_NET_WORK_QUERY	    9	//查询文件
#define		SD_NET_UPDATA_FILE		10	//更新文件管理信息
//非文件操作
#define		SD_UPGRADE_SYS		    11	//SD升级系统,包括DSP,FPGA,ARM
#define		SD_FORMAT			    12	//格式化
#define		SD_DISK_WORK_END		13  //操作结束

typedef struct tag_write_buffer
{
	char * wbuf;
	unsigned int length;
	char validSign;

}write_buffer_t;

//本地时间
typedef struct tag_local_time
{
	int sec;
	int tmin;
	int hour;
	int day;
	int mon;
	int year;
	int week;
}local_time_t;

//sd卡里文件管理信息
typedef struct tag_sd_file_manage
{
	unsigned int unload_file_num;   //未上传文件个数
	unsigned int unload_file_size;  //未上传文件大小	
}sd_file_manage_t;

//消息队列结构体
typedef struct tag_sd_message
{
	char type;  //消息类型；eg.read write
	unsigned char priority;  //优先级
	unsigned char step;  //消息执行步骤
	char valid_sign;
}sd_message_t;

//
typedef struct tag_sd_message_array_manage
{
	sd_message_t sd_message_array[MESSAGE_NUM];
	sd_message_t sd_file_manage_current;  //当前操作消息
	int count;  //数组中有效元素个数
	int index;
}sd_message_array_manage_t;
#endif