// sloCreateIndexAgent.cpp: implementation of the sloCreateIndexAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FsUiTest.h"
#include "sloCreateIndexAgent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

sloCreateIndexAgent::sloCreateIndexAgent()
{
	m_cMaxDriver = 'C';
	memset(m_szSearchName, NULL, MAX_PATH);
	m_uMaxFreeSpace.QuadPart = 0;
}

sloCreateIndexAgent::~sloCreateIndexAgent()
{

}

void sloCreateIndexAgent::GetDrvSpaceInfo(char* pDisk) 
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
		// 		CStatic* pTotalStatic = (CStatic*)GetDlgItem(IDC_TOTAL);
		// 		CStatic* pFreeStatic = (CStatic*)GetDlgItem(IDC_FREE);
		// 		pTotalStatic->SetWindowText(strTotalBytes);
		// 		pFreeStatic->SetWindowText(strFreeBytes);
	}
	
}

void sloCreateIndexAgent::FindAllDrivers()
{
	//	CComboBox* Driver=(CComboBox*)GetDlgItem(IDC_DRIVER);
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

void sloCreateIndexAgent::BuildSearchName()
{
	//�����ݿ��л�ȡ�Ѿ����ڵ�searchname�ĸ���
	int nNameCount = 0;
	sprintf(m_szSearchName,"searcher%d",nNameCount);

}

void sloCreateIndexAgent::BuildIndexPath()
{
	//�����ݿ��л�ȡ�Ѿ����ڵ�searchname�ĸ���
	char szIndexPath[MAX_PATH] = {0};
	int nNameCount = 0;
	sprintf(szIndexPath,"%c:\\searcher_%d_index",m_cMaxDriver,nNameCount);	
	
	if (CreateDirectory(szIndexPath, NULL))
	{
		//�����ļ�������
		SetFileAttributes(szIndexPath,FILE_ATTRIBUTE_HIDDEN);
	}

}
