#ifndef __SD_WORK_H
#define __SD_WORK_H

#define MESSAGE_NUM 24    //��Ϣ�����Ŀ

//ע��д����δ��������ֻ�ܱ���ʽ�����
#define		MSG_FORMAT_PRIORITY	  6
#define		MSG_DELFILE_PRIORITY  5
#define		MSG_WRITE_PRIORITY	  5
#define		MSG_UPGRADE_PRIORITY  5
#define		MSG_READ_PRIORITY	  4
#define		MSG_QUERY_PRIORITY	  3
#define		MSG_LOCK_PRIORITY	  3


//ָ��SD����״̬
#define		SD_DISK_WORK_NEW		1	//������ʼ
//�����ļ�����
#define		SD_DISK_FILE_CREATE		2   
#define		SD_DISK_FILE_WRITE		3
#define		SD_DISK_FILE_OPEN		4
#define		SD_DISK_FILE_READ		5
#define		SD_DISK_FILE_CLOSE		6
//���緢�����Ĳ���
#define		SD_NET_MANAGE_LOCK		7	//����
#define		SD_NET_MANAGE_UNLOCK	8	//����
#define		SD_NET_WORK_QUERY	    9	//��ѯ�ļ�
#define		SD_NET_UPDATA_FILE		10	//�����ļ�������Ϣ
//���ļ�����
#define		SD_UPGRADE_SYS		    11	//SD����ϵͳ,����DSP,FPGA,ARM
#define		SD_FORMAT			    12	//��ʽ��
#define		SD_DISK_WORK_END		13  //��������

typedef struct tag_write_buffer
{
	char * wbuf;
	unsigned int length;
	char validSign;

}write_buffer_t;

//����ʱ��
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

//sd�����ļ�������Ϣ
typedef struct tag_sd_file_manage
{
	unsigned int unload_file_num;       //δ�ϴ��ļ�����
	unsigned int not_updata_file_num;   //δ�����ļ�����
	unsigned int unload_file_size;      //δ�ϴ��ļ���С	
	unsigned int not_updata_file_size;  //δ�����ļ�����
}sd_file_manage_t;

//��Ϣ���нṹ��
typedef struct tag_sd_message
{
	char type;               //��Ϣ���ͣ�eg.read write
	unsigned char priority;  //���ȼ�
	unsigned char step;      //��Ϣִ�в���
	char valid_sign;
}sd_message_t;

//
typedef struct tag_sd_message_array_manage
{
	sd_message_t sd_message_array[MESSAGE_NUM];
	sd_message_t sd_file_manage_current;  //��ǰ������Ϣ
	int count;  //��������ЧԪ�ظ���
	int index;
}sd_message_array_manage_t;
#endif