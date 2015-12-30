/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "diskio.h"
#include <windows.h>  
#include <Winioctl.h>  
  
    
static HANDLE hDevice = NULL;  
   
 
/*
����״̬��ȡ
in:none
*/
DSTATUS disk_status(
	BYTE Drive     /* ���̺�*/
	)
{
	DSTATUS DiskStatus = 0;  //��ʼ������״̬Ϊδ��ʼ�� 

	return DiskStatus;
}


/* 
���̳�ʼ�� 
*/  
DSTATUS disk_initialize(  
  BYTE Drive           /* ���̺�*/  
)  
{  
	DSTATUS DiskStatus;  //��ʼ������״̬Ϊδ��ʼ��

    // TODO: Add your control notification handler code here  
	hDevice = CreateFile(L"\\\\.\\G:",
                        GENERIC_READ|GENERIC_WRITE,           //����Դ�Ķ�д����Ȩ��
                        FILE_SHARE_READ | FILE_SHARE_WRITE,   //����ģʽ
                        NULL, OPEN_EXISTING, 0, NULL  
                    );   
    if (hDevice == INVALID_HANDLE_VALUE)   
    {  
		printf("%d\n", GetLastError());

        DiskStatus = STA_NOINIT;  
        return DiskStatus;  
    }  
  
    return 0;  
}  
  
  
 
  
  
/* 
������ 
out :RES_OK (0)  �ɹ���ȡ���� 
RES_ERROR  ���������д����� 
RES_PARERR  �������� 
RES_NOTRDY ����û�г�ʼ�� 
*/  
DRESULT disk_read (  
  BYTE Drive,          /* ���̺� */  
  BYTE* Buffer,        /* ��ȡ���ݻ��� */  
  DWORD SectorNumber,  /* ������*/  
  UINT SectorCount     /* ������ */
)  
{  
    DWORD bytesread = 0;  
  
    SetFilePointer (hDevice, SectorNumber*512, 0, FILE_BEGIN);  
      
    ReadFile (hDevice, Buffer, SectorCount*512, &bytesread, NULL);  
    return RES_OK;  
  
}  
  
 
/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
DRESULT disk_write (  
  BYTE Drive,          /* ���̺� */  
  const BYTE* Buffer,        /* ��ȡ���ݻ��� */  
  DWORD SectorNumber,  /* ������*/  
  UINT SectorCount     /* ������ */
)  
{  
	DWORD dwByteReturned;
    DWORD bytesread = 0;  
    SetFilePointer (hDevice, SectorNumber*512, 0, FILE_BEGIN);  
	DeviceIoControl(
		hDevice,           
		FSCTL_LOCK_VOLUME, //win7��Ҫ�������������޷�д��       
		NULL,                       
		0,                          
		NULL, 
		0,
		&dwByteReturned,
		NULL
					);
    WriteFile (hDevice, Buffer, SectorCount*512, &bytesread, NULL);  
	DeviceIoControl(
		hDevice,           
		FSCTL_UNLOCK_VOLUME, //������Ҫ����       
		NULL,                       
		0,                          
		NULL, 
		0,
		&dwByteReturned,
		NULL
					);
    return RES_OK;  
}  
  

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DRESULT disk_ioctl (  
  BYTE Drive,      /* ���̺� */  
  BYTE Command,    /* ���� */  
  void* Buffer     /* ������ */  
)  
{  
    DISK_GEOMETRY   dg;  
    DWORD ret = 0;  
  
    switch(Command)  
    {  
    case GET_SECTOR_COUNT:  
        DeviceIoControl(hDevice,   
                IOCTL_DISK_GET_DRIVE_GEOMETRY,  
                NULL,                          // lpInBuffer  
                0,                             // nInBufferSize  
                &dg,  
                sizeof(DISK_GEOMETRY),  
                &ret,  
                NULL  
            );  
        *(int *)Buffer = dg.Cylinders.LowPart * dg.TracksPerCylinder * dg.SectorsPerTrack;//������������  
        break;  
  
    case GET_SECTOR_SIZE:  
        *(int *)Buffer = 512;//winƽ̨�ϵ�����Ϊ512��ֱ�ӷ���  
  
    }  
    return RES_OK;  
}  
  

//�޸��ļ�ʱ��
DWORD get_fattime (void)  
{  
	SYSTEMTIME tm;

	/* Get local time */
	GetLocalTime(&tm);

	/* Pack date and time into a DWORD variable */
	return 	  ((DWORD)(tm.wYear - 1980) << 25)
		| ((DWORD)tm.wMonth << 21)
		| ((DWORD)tm.wDay << 16)
		| (WORD)(tm.wHour << 11)
		| (WORD)(tm.wMinute << 5)
		| (WORD)(tm.wSecond >> 1);

}  