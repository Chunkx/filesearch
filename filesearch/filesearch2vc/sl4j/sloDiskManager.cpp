// sloDiskManager.cpp: implementation of the sloDiskManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sl4j.h"
#include "sloDiskManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

sloDiskManager::sloDiskManager()
{
	m_cMaxDriver = 'C';
	m_uMaxFreeSpace.QuadPart = 0;
}

sloDiskManager::~sloDiskManager()
{

}

void sloDiskManager::GetDrvSpaceInfo(char* pDisk) 
{
	
	//���ѡ���������
	CString Driver = pDisk;
	
	if (Driver == "C:\\")
	{
		return ;
	}
	
	//��ô��̿ռ���Ϣ
	ULARGE_INTEGER FreeAv,TotalBytes,FreeBytes;
	if(GetDiskFreeSpaceEx(Driver,&FreeAv,&TotalBytes,&FreeBytes))
	{
		//��ʽ����Ϣ������ʾ����
		CString strTotalBytes,strFreeBytes;
		strTotalBytes.Format("%u�ֽ�",TotalBytes.QuadPart);
		strFreeBytes.Format("%u�ֽ�",FreeBytes.QuadPart);
		if (FreeBytes.QuadPart > m_uMaxFreeSpace.QuadPart)
		{
			m_uMaxFreeSpace = FreeBytes;
			m_cMaxDriver = pDisk[0];
		}
	}
}

void sloDiskManager::FindAllDrivers()
{
	DWORD dwNumBytesForDriveStrings;//ʵ�ʴ洢�������ŵ��ַ�������
	HANDLE hHeap;
	LPSTR lp;
	CString strLogdrive;
	
	//���ʵ�ʴ洢�������ŵ��ַ�������
	dwNumBytesForDriveStrings=GetLogicalDriveStrings(0,NULL)*sizeof(TCHAR);
	
	//����ַ�����Ϊ�գ����ʾ������������������
	if (dwNumBytesForDriveStrings!=0) {
		//�����ַ����ռ�
		hHeap=GetProcessHeap();
		lp=(LPSTR)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,
			dwNumBytesForDriveStrings);
		
		//��ñ����������������ַ���
		GetLogicalDriveStrings(HeapSize(hHeap,0,lp),lp);
		
		//��������һ�����ŵ���������
		while (*lp!=0) {
			//Driver->AddString(lp);
			GetDrvSpaceInfo(lp);
			
			lp=_tcschr(lp,0)+1;
		}
	}
	
}


char sloDiskManager::GetMaxFreeSpaceDisk()
{
	FindAllDrivers();

	return m_cMaxDriver;
}
